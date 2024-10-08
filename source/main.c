#include "indentation.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <getopt.h>
#define OARENA_IMPLEMENTATION
#include "OArena.h"
#include "tree_navigator.h"
#include "ruler.h"
#include "node_printer.h"
#include "truncate.h"
#include "OArg.h"
#define OSTR_IMPLEMENTAION
#include "OStr.h"
#define REGEX_SIGNED_COMMENT_IMPLEMENTATION
#include "Regex_signedComment.h"
#define OJOB_IMPLEMENTATION
#include "OJob.h"
#define NOB_IMPLEMENTATION
#include "../nob.h"

// Declare the `tree_sitter_json` function, which is
// implemented by the `tree-sitter-json` library.
TSLanguage *tree_sitter_c();

/* return true => success */ 
bool read_txt_file(OStr *source, char const *path) {
        FILE *file = fopen(path, "r");
        if (file == NULL) {
                fprintf(stderr, "`%s` Error %d %s\n", path, errno, strerror(errno));
                return false;
        }
        source->size = 0;
        source->at[0] = 0;
        char chr;
        do {
                chr = getc(file);
                source->at[source->size] = chr;
                source->size++; 
        } while (chr != EOF);
        source->size--; 
        source->at[source->size] = 0;
        fclose(file);
        return true;
}

/* return true => successful */
bool write_txt_file(OStr const *source, char const *path) {
        if (strcmp(path,"-") == 0) { 
                printf("source_code:\n%s", source->at);
                return true;
        } else {
                FILE *file = fopen(path, "w");
                if (file == NULL) {
                        return false;
                }
                fprintf(file, "%s", source->at);
                fclose(file);
                return true;
        }
}

#define ARENA
OArena *memory_for_treesitter;
void *ooo_malloc(size_t size) {
#ifndef ARENA
        void *p = malloc(size);
        // printf("malloc %p %zu \n ", p, size);
        return p;
#else
        // printf("OArena_malloc -> size %zu \n", size);
        void *p = OArena_malloc(memory_for_treesitter, size);
        // printf("OArena_malloc -> %p size %zu \n", p, memory_for_treesitter->size);
        return p;
#endif
}

void *ooo_calloc(size_t nitems, size_t size) {
#ifndef ARENA
        // printf("calloc %zu %zu \n ", nitems, size);
        return calloc(nitems, size);
#else
        // printf("OArena_calloc -> new_nitems %zu new_size %zu \n", nitems, size);
        void *p = OArena_calloc(memory_for_treesitter, nitems, size);
        // printf("OArena_calloc -> %p size %zu \n", p, memory_for_treesitter->size);
        return p;
#endif
}

void *ooo_realloc(void *buffer, size_t size) {
#ifndef ARENA
        // printf("reallo %p %zu <----------------------\n ", buffer, size);
        return realloc(buffer, size);
#else
        // printf("OArena_realloc -> %p new_size %zu \n", buffer, size);
        void *p = OArena_realloc(memory_for_treesitter, buffer, size);
        // printf("OArena_realloc -> %p size %zu \n", p, memory_for_treesitter->size);
        return p;
#endif
}

void ooo_free(void *buffer) {
#ifndef ARENA
        // printf("free %p \n", buffer);
        free(buffer);
#else
        // printf("OArena_free %p\n", buffer);
        OArena_free(memory_for_treesitter, buffer);
#endif
}


Nodes serial_nodes;
#define MEM_SIZE 1024*1024

int main(int argc, char **argv) {
        uint64_t t_start = nob_millis();
        OArg_t oarg = {0};
        if (OArg_init(&oarg, argc, argv)) {
                return 1;
        }

        char *snk = (char*)malloc(MEM_SIZE);
        char *src = (char*)malloc(MEM_SIZE);
        OJob job = {
                .idx = 0,
                .sink = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = snk
                },
                .source = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = src
                },
        };

        if (!read_txt_file(&job.source, oarg.input_path)) return 2;

        char const NEW_LINE = OStr_set_NewLine_with_LineFeed(&job.sink, &job.source);
        OStr_replace_tabs_with_one_space(&job.source, &job.sink);

        memory_for_treesitter = OArena_make(10 * 1024 * 1024);
        ts_set_allocator(ooo_malloc, ooo_calloc, ooo_realloc, ooo_free);
        TSParser *parser = ts_parser_new();
        ts_parser_set_language(parser, tree_sitter_c());
        {
                TSTree *tree = ts_parser_parse_string(
                        parser,
                        NULL,
                        job.source.at,
                        job.source.size
                );
                ooo_truncate_spaces(ts_tree_root_node(tree), &job); 
                OJob_swap(&job);
                ts_tree_delete(tree);
        }
        {
                TSTree *tree = ts_parser_parse_string(
                        parser,
                        NULL,
                        job.source.at,
                        job.source.size
                );
                if (oarg.action == OARG_PRINT) { 
                        ooo_print_nodes(
                                ts_tree_root_node(tree),
                                oarg.print.row_begin,
                                oarg.print.row_end, 
                                0 // tree-branch level
                        );
                        return 0;
                }
                serial_nodes = Nodes_init(20);
                Nodes_push(&serial_nodes, ts_tree_root_node(tree));
                ooo_ruler(
                        &serial_nodes,
                        &job
                );
                OJob_swap(&job);
                ts_tree_delete(tree);
        }
        {
                TSTree *tree = ts_parser_parse_string(
                        parser,
                        NULL,
                        job.source.at,
                        job.source.size
                );
                Nodes_clear(&serial_nodes);
                Nodes_push(&serial_nodes, ts_tree_root_node(tree));
                ooo_set_indentation(
                        &job,
                        &serial_nodes,
                        0
                );
                OJob_swap(&job);
                OStr_replace_LineFeed(&job.sink, &job.source, NEW_LINE);
                OJob_swap(&job);
                ts_tree_delete(tree);
        }
        write_txt_file(&job.source, oarg.output_path);
        ts_parser_delete(parser);
        printf("%lu ms\n", (nob_millis() - t_start));
        return 0;
}

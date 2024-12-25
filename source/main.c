#include "indentation.h"
#include "iteration.h"
#include "Pars.h"
#include "tree_navigator.h"
#include "ruler.h"
#include "truncate.h"
#include "node_printer.h"
#include "OArg.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <getopt.h>
#define OARENA_IMPLEMENTATION
#include "OArena.h"
#define OSTR_IMPLEMENTAION
#include "OStr.h"
#define OJOB_IMPLEMENTATION
#include "OJob.h"
#define NOB_IMPLEMENTATION
#include "../nob.h"

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

OArena *arena_treesitter;
void *ooo_malloc(size_t size) {
        // printf("OArena_malloc -> size %zu \n", size);
        void *p = OArena_malloc(arena_treesitter, size);
        // printf("OArena_malloc -> %p size %zu \n", p, memory_for_treesitter->size);
        return p;
}

void *ooo_calloc(size_t nitems, size_t size) {
        // printf("OArena_calloc -> new_nitems %zu new_size %zu \n", nitems, size);
        void *p = OArena_calloc(arena_treesitter, nitems, size);
        // printf("OArena_calloc -> %p size %zu \n", p, memory_for_treesitter->size);
        return p;
}

void *ooo_realloc(void *buffer, size_t size) {
        // printf("OArena_realloc -> %p new_size %zu \n", buffer, size);
        void *p = OArena_realloc(arena_treesitter, buffer, size);
        // printf("OArena_realloc -> %p size %zu \n", p, memory_for_treesitter->size);
        return p;
}

void ooo_free(void *buffer) {
        // printf("OArena_free %p\n", buffer);
        OArena_free(arena_treesitter, buffer);
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
                .lineContinuation = 0,
                .offset = 0,
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
        
        first_iteration(&job);
        OJob_swap(&job);

        arena_treesitter = OArena_make(10 * 1024 * 1024);
        ts_set_allocator(ooo_malloc, ooo_calloc, ooo_realloc, ooo_free);
        Pars_setArena(arena_treesitter);
        {
                RootNode_t root = Pars_getTree(job.source.at, job.source.size);
                if (oarg.action == OARG_PRINT) { 
                        ooo_print_nodes(
                                root.node,
                                oarg.print.row_begin,
                                oarg.print.row_end, 
                                0 // tree-branch level
                        );
                        return 0;
                }
                ooo_truncate_spaces(root.node, &job); 
                Pars_freeTree(root);
                OJob_swap(&job);
        }
        {
                RootNode_t root = Pars_getTree(job.source.at, job.source.size);
                serial_nodes = Nodes_init(20);
                Nodes_push(&serial_nodes, root.node);
                ooo_ruler(
                        &serial_nodes,
                        &job
                );
                Pars_freeTree(root);
                OJob_swap(&job);
        }
        {
                RootNode_t root = Pars_getTree(job.source.at, job.source.size);
                Nodes_clear(&serial_nodes);
                Nodes_push(&serial_nodes, root.node);
                ooo_set_indentation(
                        &job,
                        &serial_nodes,
                        0

                );
                Pars_freeTree(root);
                OJob_swap(&job);
        }
        last_iteration(&job);
        OJob_swap(&job);

        write_txt_file(&job.source, oarg.output_path);
        printf("%lu ms\n", (nob_millis() - t_start));
        return 0;
}

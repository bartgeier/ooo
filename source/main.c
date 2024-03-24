#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include "tree_navigator.h"
#include "ruler.h"
#include "indentation.h"
#include "node_printer.h"
#include "OJob.h"
#include "truncate.h"
#define OSTR_IMPLEMENTAION
#include "OStr.h"
#include "OArg.h"


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

#define MEM_SIZE 1000*1024
int main(int argc, char **argv) {
        OArg_t oarg = {0};
        if (OArg_init(&oarg, argc, argv)) {
                return 1;
        }

        char *snk = (char*)malloc(MEM_SIZE);
        char *src = (char*)malloc(MEM_SIZE);
        OJob job = {
                .cursor = {0},
                .sink = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = snk
                },
                .source = {
                        .capacity = MEM_SIZE,
                        .size = 0,
                        .at = src
                }
        };

        if (!read_txt_file(&job.source, oarg.input_path)) return 2;


        char const NEW_LINE = OStr_set_NewLine_with_LineFeed(&job.sink, &job.source);
        OStr_replace_tabs_with_one_space(&job.source, &job.sink);

        TSParser *parser = ts_parser_new();
        ts_parser_set_language(parser, tree_sitter_c());
        TSTree *tree = ts_parser_parse_string(
                parser,
                NULL,
                job.source.at,
                job.source.size
        );
        OStrCursor_reset(&job.cursor);
        ooo_truncate_spaces(ts_tree_root_node(tree), &job); 
        OStr_move(&job.source, &job.sink);

        tree = ts_parser_parse_string(
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
                        0 // level
                );
                return 0;
        }
        OStrCursor_reset(&job.cursor);
        ooo_ruler(
                ts_tree_root_node(tree),
                ts_tree_root_node(tree),
                &job
        );

        OStr_clear(&job.source);

        OStrCursor_reset(&job.cursor);
        tree = ts_parser_parse_string(
                parser,
                NULL,
                job.sink.at,
                job.sink.size
        );
        ooo_set_indentation(
                &job.cursor,
                &job.source,
                &job.sink,
                ts_tree_root_node(tree),
                0
        );
        OStr_clear(&job.sink);
        OStr_replace_LineFeed(&job.sink, &job.source, NEW_LINE);
        write_txt_file(&job.sink, oarg.output_path);
        ts_tree_delete(tree);
        ts_parser_delete(parser);
        return 0;
}

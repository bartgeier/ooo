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

        Pars_init();
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

                Relation relation = {0}; 
                Relation_init(&relation, &serial_nodes);

                ooo_ruler(
                        &relation,
                        &job
                );
                Pars_freeTree(root);
                OJob_swap(&job);
        }
        {
                RootNode_t root = Pars_getTree(job.source.at, job.source.size);
                Nodes_clear(&serial_nodes);
                Nodes_push(&serial_nodes, root.node);

                Relation relation = {0}; 
                Relation_init(&relation, &serial_nodes);

                ooo_set_indentation(
                        &job,
                        &relation,
                        0
                );
                Pars_freeTree(root);
                OJob_set_final_LF(&job);
                OJob_swap(&job);
        }
        last_iteration(&job);
        OJob_swap(&job);

        write_txt_file(&job.source, oarg.output_path);
        printf("%lu ms\n", (nob_millis() - t_start));
        return 0;
}

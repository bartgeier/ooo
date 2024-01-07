#ifndef OARG_H
#define OARG_H

#include <stdlib.h>

typedef enum {
        OARG_STYLE,
        OARG_HELP,
        OARG_PRINT
} OArg_e;

typedef struct {
        OArg_e action;
        char *input_path;
        char *output_path;
        struct {
                int failure;
                size_t row_begin;
                size_t row_end;
        } print;
} OArg_t;

int OArg_init(OArg_t *m, int argc, char **argv);

#endif

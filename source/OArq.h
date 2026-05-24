#ifndef OARQ_H
#define OARQ_H

#include "stdlib.h"
#include "stdbool.h"

typedef enum {
        OARQ_NO_ACTION,
        OARQ_STYLE,
        OARQ_PRINT
} OArq_e;

typedef struct {
        struct {
                bool help;
                bool version;
        } flag;
        OArq_e action;
        char const *input_path;
        char const *output_path;
        struct {
                size_t row_begin;
                size_t row_end;
        } print;
} OArq_t;

int OArq_init(OArq_t *m, int argc, char **argv);


#endif

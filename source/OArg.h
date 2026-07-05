#ifndef OARQ_H
#define OARQ_H

#include "stdlib.h"
#include "stdbool.h"
#include "stdint.h"

typedef enum {
        OARG_ERROR,
        OARG_NO_ACTION,
        OARG_STYLE,
        OARG_PRINT
} OArg_e;

typedef struct {
        struct {
                bool help;
                bool version;
                bool license;
                bool print;
                bool KR;
                bool indent;
        } flag;
        char const *input_path;
        char const *output_path;
        uint32_t indent;
        struct {
                size_t row_begin;
                size_t row_end;
        } print;
        OArg_e action;
} OArg_t;

OArg_t OArg_init(int argc, char **argv);


#endif

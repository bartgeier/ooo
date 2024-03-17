#ifndef APPLY_RULES_H
#define APPLY_RULES_H

#include "tree_navigator.h"
#include "ooo_job.h"
#include <stdbool.h>

typedef struct {
        size_t begin;
        size_t end;
} OOO_Slice;

TSNode ooo_apply_rules(
        TSNode node,
        TSNode serial_node,
        OOO_Job *job
);

#endif

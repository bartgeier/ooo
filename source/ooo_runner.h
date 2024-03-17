#ifndef OOO_RUNNER_H
#define OOO_RUNNER_H

#include "tree_navigator.h"
#include "ooo_job.h"

#if 0
typedef enum {
        OOO_ENTRY,
        OOO_NEXT,
        OOO_EXIT
} OOO_Transition;
#endif

#if 0 
typedef struct {
        size_t begin;
        size_t end;
} OOO_Slice;
#endif

#if 0
void ooo_set_indentation(
        OStrCursor *cursor,
        OStr *sink,
        OStr *source,
        TSNode node,
        size_t indentation_level
);
#endif

void ooo_print_nodes(
        TSNode node,
        size_t row_begin,
        size_t row_end,
        size_t level
); 

#if 0
TSNode ooo_visit_nodes(
        TSNode node,
        TSNode serial_node,
        OOO_Job *job
);

bool ooo_rule_dispatcher(
        TSNode node,
        TSNode serial_node,
        OOO_Slice slice,
        OOO_Job *job
);
#endif

#if 0
size_t ooo_indentation(OOO_Transition const transition, TSNode const node, size_t level);
#endif

#endif

#ifndef OOO_RUNNER_H
#define OOO_RUNNER_H

#include "tree_sitter/api.h"
#include "ooo_job.h"

typedef enum {
        OOO_ENTRY,
        OOO_NEXT,
        OOO_EXIT
} OOO_Transition;


typedef struct {
        size_t begin;
        size_t end;
} OOO_Slice;

void ooo_set_indentation(
        OStrCursor *cursor,
        OStr *sink,
        OStr *source,
        TSNode node,
        size_t indentation_level
);

void ooo_print_nodes(
        TSNode node,
        size_t row_begin,
        size_t row_end,
        size_t level
); 

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

size_t ooo_indentation(OOO_Transition const transition, TSNode const node, size_t level);

TSSymbol ooo(TSNode);
TSNode super(int i, TSNode n); 
TSNode child(unsigned int const i, TSNode n);
TSNode child_first(TSNode n);
TSNode child_last(TSNode n);
TSNode sibling(int i, TSNode n);

#endif

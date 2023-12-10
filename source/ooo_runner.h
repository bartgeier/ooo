#ifndef OOO_RUNNER_H
#define OOO_RUNNER_H

#include "tree_sitter/api.h"
#include "treesitter_symbol_ids.h"
#include "StringView.h"

typedef enum {
        OOO_ENTRY,
        OOO_NEXT,
        OOO_EXIT
} OOO_Transition;

typedef struct {
        OStrCursor cursor;
        OStr sink;
        OStr source;
        size_t indentation_level;
} OOO_Job;

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

#endif

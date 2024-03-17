#ifndef INDENTATION_H
#define INDENTATION_H

#include "OStr.h"
#include "tree_navigator.h"

typedef enum {
        OOO_ENTRY,
        OOO_NEXT,
        OOO_EXIT
} OOO_Transition;

void ooo_set_indentation(
        OStrCursor *cursor,
        OStr *sink,
        OStr *source,
        TSNode node,
        size_t indentation_level
);

//size_t ooo_indentation(OOO_Transition const transition, TSNode const node, size_t level);

#endif

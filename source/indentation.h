#ifndef INDENTATION_H
#define INDENTATION_H

#include "OStr.h"
#include "tree_navigator.h"
#include "OJob.h"

typedef enum {
        OOO_ENTRY,
        OOO_NEXT,
        OOO_EXIT
} OOO_Transition;

#if 1
void ooo_set_indentation(
        OStrCursor *cursor,
        OStr *sink,
        OStr *source,
        TSNode node,
        size_t indentation_level
);
#else
void ooo_set_indentation(
        OJob *job,
        TSNode node,
        size_t indentation_level
);
#endif

//size_t ooo_indentation(OOO_Transition const transition, TSNode const node, size_t level);

#endif

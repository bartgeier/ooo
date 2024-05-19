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

void ooo_set_indentation(
        OJob *job,
        Nodes *nodes,
        size_t indentation_level
);

#endif

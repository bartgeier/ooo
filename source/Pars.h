#ifndef PARS_H
#define PARS_H

#include "tree_navigator.h"
#include "OArena.h"

typedef struct {
        uint32_t idx; // OArena idx
        TSNode node;
} RootNode_t;

void Pars_setArena(OArena *a);
RootNode_t Pars_getTree(char const *string, uint32_t size);
void Pars_freeTree(RootNode_t root);

#endif

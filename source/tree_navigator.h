#ifndef TREE_NAVIGATOR_H
#define TREE_NAVIGATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tree_sitter/api.h"
#include "ooo_treesitter_symbol_ids.h"


bool is_single_line(TSNode n);
TSSymbol ooo(TSNode);
TSNode super(int i, TSNode n); 
TSNode child(unsigned int const i, TSNode n);
TSNode child_first(TSNode n);
TSNode child_last(TSNode n);
TSNode sibling(int i, TSNode n);

typedef struct {
        size_t SIZE;
        size_t last;
        TSNode *at;
} Nodes;

Nodes Nodes_init(size_t const SIZE);
void Nodes_push(Nodes *m, TSNode const node);
TSNode Nodes_at(Nodes *m, size_t const idx);

#ifdef __cplusplus
}
#endif

#endif

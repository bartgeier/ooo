#ifndef TREE_NAVIGATOR_H
#define TREE_NAVIGATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tree_sitter/api.h"
#include "ooo_treesitter_symbol_ids.h"

TSSymbol sym(TSNode n);
bool is_single_line(TSNode n);
TSNode super(int i, TSNode n); 
TSNode sibling(int i, TSNode n);

typedef struct {
        uint32_t SIZE;
        uint32_t last;
        TSNode *at;
} Nodes;

Nodes Nodes_init(uint32_t const SIZE);
void Nodes_clear(Nodes *m);
void Nodes_push(Nodes *m, TSNode const node);
TSNode Nodes_at(Nodes const *m, uint32_t const idx);

typedef struct {
        TSNode grand;
        TSNode parent;
        uint32_t child_idx;
        uint32_t num_of_childs;
        Nodes const *nodes;
} Relation;

void Relation_init(Relation *r, Nodes const *nodes);
TSNode child(Relation const *r, unsigned int const i);
TSSymbol unknown(Relation const *r);
TSSymbol me(Relation const *r);
uint32_t me_size(Relation const *r);
TSSymbol parent(Relation const *r);
TSSymbol grand(Relation const *r);
bool is_error(Relation const *r);
bool is_first_child(Relation const *r);
bool is_last_child(Relation const *r);
bool is_after_child(TSSymbol symbol, Relation const *r);
bool is_middle_child(TSSymbol symbol_a, Relation const *r, TSSymbol symbol_b);
bool is_before_child(Relation const *r, TSSymbol symbol);
int find_child(Relation const *r, TSSymbol const symbol);

#ifdef __cplusplus
}
#endif

#endif

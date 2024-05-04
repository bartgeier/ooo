#ifndef TREE_NAVIGATOR_H
#define TREE_NAVIGATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tree_sitter/api.h"
#include "ooo_treesitter_symbol_ids.h"


bool node_end_with_LF(TSSymbol const symbol);
bool is_single_line(TSNode n);
TSSymbol ooo(TSNode);
TSNode super(int i, TSNode n); 
int find_child(TSNode const parent, TSSymbol const symbol);
TSNode child(unsigned int const i, TSNode n);
TSNode child_first(TSNode n);
TSNode child_last(TSNode n);
TSNode sibling(int i, TSNode n);
bool first_sibling(TSNode const node);
bool second_sibling(TSNode const node);
bool third_sibling(TSNode const node);
bool fourth_sibling(TSNode const node);
bool fifth_sibling(TSNode const node);
bool sixth_sibling(TSNode const node);
bool seventh_sibling(TSNode const node);
bool last_sibling(TSNode const node);

typedef struct {
        size_t SIZE;
        size_t last;
        TSNode *at;
} Nodes;


Nodes Nodes_init(size_t const SIZE);
void Nodes_push(Nodes *m, TSNode const node);
TSNode Nodes_at(Nodes *m, size_t const idx);

typedef struct {
        TSNode grand;
        TSNode parent;
        uint32_t child_idx;
        uint32_t num_of_childs;
        Nodes *nodes;
} Relation;

void Relation_init(Relation *r, Nodes *nodes);
TSSymbol me(Relation const *r);
TSSymbol parent(Relation const *r);
bool is_first_child(Relation const *r);
bool is_last_child(Relation const *r);
bool is_after_child(TSSymbol symbol, Relation const *r);
bool is_middle_child(TSSymbol symbol_a, Relation const *r, TSSymbol symbol_b);
bool is_before_child(Relation const *r, TSSymbol symbol);
bool has_child(Relation const *r, TSSymbol symbol);
uint32_t me_size(Relation const *r);
uint32_t parent_num_of_lines(Relation const *r);

#ifdef __cplusplus
}
#endif

#endif

#ifndef TREE_NAVIGATOR_H
#define TREE_NAVIGATOR_H

#include "tree_sitter/api.h"
#include "ooo_treesitter_symbol_ids.h"

bool is_single_line(TSNode n);
TSSymbol ooo(TSNode);
TSNode super(int i, TSNode n); 
TSNode child(unsigned int const i, TSNode n);
TSNode child_first(TSNode n);
TSNode child_last(TSNode n);
TSNode sibling(int i, TSNode n);

#endif

#include "tree_navigator.h"
#include <assert.h>

/* ts_node__null is unfortunately not public in tree-sitter api.h */
/* see also tree-sitter/lib/src/node.c ts_node__null              */
static inline TSNode node_null(void) {
#ifdef __cplusplus //because of google-test
        TSNode n;
        for (size_t i = 0; i < 4; i++) { n.context[i] = 0; }
        n.id = NULL;
        n.tree = NULL;
        return n;
#else
        return (TSNode){{0, 0, 0, 0}, NULL, NULL};
#endif
}

bool is_single_line(TSNode n) {
        TSPoint const start = ts_node_start_point(n);
        TSPoint const end = ts_node_end_point(n);
        return start.row == end.row;
}

TSSymbol ooo(TSNode n) {
        if (ts_node_is_null(n)) {
                return 0;
        }
        return ts_node_symbol(n);
}

TSNode super(int i, TSNode n) {
        while (i > 0 & !ts_node_is_null(n)) {
                n = ts_node_parent(n);
                i--;
        }
        return n;
}

TSNode sibling(int i, TSNode n) {
        while (i > 0 & !ts_node_is_null(n)) {
                n = ts_node_next_sibling(n);
                i--;
        }
        while (i < 0 & !ts_node_is_null(n)) {
                n = ts_node_prev_sibling(n);
                i++;
        }
        return n;
}

Nodes Nodes_init(size_t const SIZE) {
        Nodes m = {
                .SIZE = SIZE,
                .last = 0,
                .at = 0 
        };
        m.at = (TSNode*)calloc(m.SIZE, sizeof(TSNode));
        for (size_t i = 0; i < m.SIZE; i++) {
                m.at[i] = node_null();
        }
        return m;
}

void Nodes_push(Nodes *m, TSNode const node) {
        m->last = (m->last + 1) % m->SIZE;
        m->at[m->last] = node;
}

TSNode Nodes_at(Nodes *m, size_t const idx) {
        assert(idx < m->SIZE);
        size_t i = (m->last >= idx) ? m->last - idx : m->SIZE - idx + m->last;
        return m->at[i];
}

void Relation_init(Relation *r, Nodes *nodes) {
        r->nodes = nodes;
        TSNode node = Nodes_at(nodes, 0);
        r->parent = super(1, node);
        if (ts_node_is_null(r->parent)) {
                return;
        }
        r->grand = super(1, r->parent);
        r->num_of_childs = ts_node_child_count(r->parent);
        for (uint32_t i = 0; i < r->num_of_childs; i++) {
                if (node.id == ts_node_child(r->parent, i).id) {
                        r->child_idx = i;
                        return;
                }
        }
        r->child_idx = 0;
}


TSNode child(Relation const *r, unsigned int const i) {
        if (!ts_node_is_null(r->parent)) {
                return ts_node_child(r->parent, i);
        }
        return r->parent;
}

TSSymbol unknown(Relation const *r) {
        return SYM_UNKNOWN(me(r));
}

TSSymbol me(Relation const *r) {
        return ooo(Nodes_at(r->nodes, 0));
}

TSSymbol parent(Relation const *r) {
        return ooo(r->parent);
}

bool is_error(Relation const *r) {
        TSNode n = Nodes_at(r->nodes, 0);
        return ts_node_has_error(n) || ts_node_is_error(n);
}

bool is_first_child(Relation const *r) {
        return r->child_idx == 0;
}

bool is_last_child(Relation const *r) {
        return r->child_idx == r->num_of_childs - 1;
}

bool is_after_child(TSSymbol symbol, Relation const *r) {
        for (uint32_t i = r->child_idx - 1; i < r->num_of_childs; i--) {
                if (ooo(ts_node_child(r->parent, i)) == symbol) {
                        return true;
                }
        }
        return false;
}

bool is_middle_child(TSSymbol symbol_a, Relation const *r, TSSymbol symbol_b) {
        return is_after_child(symbol_a, r) && is_before_child(r, symbol_b); 
}

bool is_before_child(Relation const *r, TSSymbol symbol) {
        for (uint32_t i = r->child_idx + 1; i < r->num_of_childs; i++) {
                if (ooo(ts_node_child(r->parent, i)) == symbol) {
                        return true;
                }
        }
        return false;
}

uint32_t me_size(Relation const *r) {
        TSNode n = Nodes_at(r->nodes, 0);
        return (ts_node_end_byte(n) - ts_node_start_byte(n));
}

/* return -1 not found => return >= 0 found child idx */
int find_child(Relation const *r, TSSymbol const symbol) {
        for (size_t i = 0; i < r->num_of_childs; i++) {
                if (ooo(ts_node_child(r->parent, i)) == symbol) {
                       return i;
               }
        }
        return -1;
}

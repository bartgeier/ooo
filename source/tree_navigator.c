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

TSNode child(unsigned int const i, TSNode n) {
        if (!ts_node_is_null(n)) {
                n = ts_node_child(n, i);
        }
        return n;
}

TSNode child_first(TSNode n) {
        if (!ts_node_is_null(n)) {
                n = ts_node_child(n, 0);
        }
        return n;
}

TSNode child_last(TSNode n) {
        if (!ts_node_is_null(n)) {
                n = ts_node_child(n, ts_node_child_count(n) - 1);
        }
        return n;
}

TSNode sibling(int i, TSNode n) {
        while (i > 0 & !ts_node_is_null(n)) {
                n = ts_node_prev_sibling(n);
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

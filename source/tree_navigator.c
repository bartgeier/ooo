#include "tree_navigator.h"

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
        //for (size_t it = 0; it < num_of_childs; it++) {
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

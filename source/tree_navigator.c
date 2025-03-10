#include "tree_navigator.h"
#include <assert.h>

/* ts_node__null is unfortunately not public in tree-sitter api.h */
/* see also tree-sitter/lib/src/node.c ts_node__null              */
static inline TSNode node_null(void) {
#ifdef __cplusplus //because of google-test
        TSNode n;
        for (uint32_t i = 0; i < 4; i++) { n.context[i] = 0; }
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

TSSymbol sym(TSNode n) {
        if (ts_node_is_null(n)) {
                return 0;
        }
        return ts_node_symbol(n);
}

TSNode super(int i, TSNode n) {
        while ((i > 0) & !ts_node_is_null(n)) {
                n = ts_node_parent(n);
                i--;
        }
        return n;
}

TSNode sibling(int i, TSNode n) {
        while ((i > 0) & !ts_node_is_null(n)) {
                n = ts_node_next_sibling(n);
                i--;
        }
        while ((i < 0) & !ts_node_is_null(n)) {
                n = ts_node_prev_sibling(n);
                i++;
        }
        return n;
}

TSNode Relation_track_node(Relation const *r, uint32_t const idx) {
        Track const *tracks = &r->tracks;
        assert(idx < tracks->SIZE);
        uint32_t i = (tracks->last >= idx) ? tracks->last - idx : tracks->SIZE - idx + tracks->last;
        return tracks->at[i];
}

Relation Relation_make(uint32_t const SIZE) {
        Relation r = { 
                .ggrand = node_null(),
                .grand = node_null(),
                .parent = node_null(),
                .child_idx = 0,
                .num_of_childs = 0,
                .tracks = {
                        .SIZE = SIZE,
                        .last = 0,
                        .at = NULL 
                }
        };
        r.tracks.at = (TSNode*)calloc(r.tracks.SIZE, sizeof(TSNode));
        for (uint32_t i = 0; i < r.tracks.SIZE; i++) {
                r.tracks.at[i] = node_null();
        }
        return r;
}

void Relation_clear(Relation *r) {
        r->ggrand = node_null(),
        r->grand = node_null(),
        r->parent = node_null(),
        r->child_idx = 0;
        r->num_of_childs = 0;
        r->tracks.last = 0;
        for (uint32_t i = 0; i < r->tracks.SIZE; i++) {
                r->tracks.at[i] = node_null();
        }
}

// track: serialices the nodes in the same order, they are written in the c file.
void Relation_track(Relation *r, TSNode const node, uint32_t const child_idx) {
        r->tracks.last = (r->tracks.last + 1) % r->tracks.SIZE;
        r->tracks.at[r->tracks.last] = node;
        r->child_idx = child_idx;
}

void Relation_parent_push(Relation *r, TSNode const parent, uint32_t const num_of_childs, TSNode const grand, TSNode const ggrand) {
        r->ggrand = ggrand;
        r->grand = grand;
        r->parent = parent;
        r->num_of_childs = num_of_childs;
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
        return sym(Relation_track_node(r, 0));
}

TSSymbol parent(Relation const *r) {
        return sym(r->parent);
}

TSSymbol grand(Relation const *r) {
        return sym(r->grand);
}

TSSymbol ggrand(Relation const *r) {
        return sym(r->ggrand);
}

bool is_error(Relation const *r) {
        TSNode n = Relation_track_node(r, 0);
        return ts_node_has_error(n) || ts_node_is_error(n);
}

bool is_first_child(Relation const *r) {
        return r->child_idx == 0;
}

bool is_second_child(Relation const *r) {
        return r->child_idx == 1;
}

bool is_last_child(Relation const *r) {
        return r->child_idx == r->num_of_childs - 1;
}

bool is_after_child(TSSymbol symbol, Relation const *r) {
        for (uint32_t i = r->child_idx - 1; i < r->num_of_childs; i--) {
                if (sym(ts_node_child(r->parent, i)) == symbol) {
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
                if (sym(ts_node_child(r->parent, i)) == symbol) {
                        return true;
                }
        }
        return false;
}

uint32_t me_size(Relation const *r) {
        TSNode n = Relation_track_node(r, 0);
        return (ts_node_end_byte(n) - ts_node_start_byte(n));
}

/* return -1 not found => return >= 0 found child idx */
int find_child(Relation const *r, TSSymbol const symbol) {
        for (uint32_t i = 0; i < r->num_of_childs; i++) {
                if (sym(ts_node_child(r->parent, i)) == symbol) {
                       return i;
               }
        }
        return -1;
}

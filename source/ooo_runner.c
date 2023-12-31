#include "ooo_runner.h"
#include <stdio.h>

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

TSNode ooo_visit_nodes(
        TSNode node,
        TSNode serial_node,
        OOO_Job *job
) {
        static size_t count = 0;

        TSPoint start_point = ts_node_start_point(node);
        OOO_Slice slice = {
                .begin = job->cursor.idx,
                .end = OStrCursor_move_to_point(&job->cursor, &job->source, start_point)
        };

        if (ooo_rule_dispatcher(node, serial_node, slice, job)) {
                for (size_t i = slice.begin; i < slice.end; i++) {
                        OStr_append_chr(&job->sink, job->source.at[i]);
                }
        }

        size_t num_of_childs = ts_node_child_count(node);
        TSNode serial = node;
        for (size_t it = 0; it < num_of_childs; it++) {
               TSNode child = ts_node_child(node, it);
                serial = ooo_visit_nodes(
                        child,
                        serial,
                        job
                );
        }

        TSPoint end_point = ts_node_end_point(node);
        slice.begin = job->cursor.idx;
        slice.end = OStrCursor_move_to_point(&job->cursor, &job->source, end_point);
        for (size_t i = slice.begin; i < slice.end; i++) {
                OStr_append_chr(&job->sink, job->source.at[i]);
        }
        return serial;
}


void ooo_set_indentation(
        OStrCursor *cursor,
        OStr *sink,
        OStr *source,
        TSNode node,
        size_t indentation_level
) {
        //char const *type_name = ts_node_type(node);
        TSSymbol me = ooo(node);

        size_t cx = cursor->idx;
        size_t sx = OStrCursor_move_to_point(
                cursor, 
                source, 
                ts_node_start_point(node)
        );
        //printf("%zu %zu \n", cx, sx);
        indentation_level = ooo_indentation(OOO_ENTRY, node, indentation_level);
        for (size_t i = cx; i < sx; i++) {
                if (source->at[i] == '\n') {
                        OStr_append_chr(sink, source->at[i]);
                        // printf("%c", source->at[i]);
                        OStr_append_spaces(sink, 4 * indentation_level);
                } else {
                        OStr_append_chr(sink, source->at[i]);
                        // printf("%c", source->at[i]);
                }
        }

        indentation_level = ooo_indentation(OOO_NEXT, node, indentation_level);
        for (size_t it = 0; it < ts_node_child_count(node); it++) {
                TSNode child = ts_node_child(node, it);
                ooo_set_indentation(
                        cursor,
                        sink,
                        source,
                        child,
                        indentation_level
                );
        }
        size_t ax = cursor->idx;
        size_t ex = OStrCursor_move_to_point(cursor, source, ts_node_end_point(node));
        if (me == sym_preproc_def 
        | me == anon_sym_LF & ooo(super(1, node)) == sym_preproc_if) {
                /* preproc_def node includes the \n                */
                /* \n is then not used for indentation             */
                /* OStrCursor_decrement set the ex index before \n */
                ex = OStrCursor_decrement(cursor, source);
        } 
        for (size_t i = ax; i < ex; i++) {
                OStr_append_chr(sink, source->at[i]);
                // printf("%c", source->at[i]);
        }
}

#if 0
void ooo_print_nodes(
        TSNode node,
        size_t row_begin,
        size_t row_end,
        size_t level
) {
        TSPoint start = ts_node_start_point(node);
        TSPoint end = ts_node_end_point(node);
        size_t num_of_childs = ts_node_child_count(node);

        if (level == 0) {
                printf("NodeID| Start  | End    | Level-> NodeName  num_of_childs\n");
        }
        if (start.row >= row_begin && start.row < row_end) {
                printf("%4d  |%4d:%-3d|%4d:%-3d|%3zu",
                        ts_node_symbol(node),
                        start.row,
                        start.column,
                        end.row,
                        end.column,
                        level
                );
                for (size_t i = 0; i < level; i++) {
                        printf("-");
                }
                printf("> %s %zu\n",
                        ts_node_type(node),
                        num_of_childs
                );
        }

        level++;
        for (size_t it = 0; it < num_of_childs; it++) {
                TSNode child = ts_node_child(node, it);
                ooo_print_nodes(child, row_begin, row_end, level);
        }
}
#else
void ooo_print_nodes(
        TSNode node,
        size_t row_begin,
        size_t row_end,
        size_t level
) {
        TSPoint start = ts_node_start_point(node);
        TSPoint end = ts_node_end_point(node);
        size_t num_of_childs = ts_node_child_count(node);

        if (level == 0) {
                printf("Start | End    |  (NodeID)me (NodeID)parent (NodeID)grand_parent ...\n");
        }
        if (start.row >= row_begin && start.row < row_end) {
                printf("%d:%-3d|%4d:%-3d|  ",
                        start.row,
                        start.column,
                        end.row,
                        end.column
                );
                TSNode x = node;
                for (size_t i = 0; i < level; i++) {
                        printf("(%d)%s ", ts_node_symbol(x), ts_node_type(x));
                        x = ts_node_parent(x);
                }
                printf("\n");
        }

        level++;
        for (size_t it = 0; it < num_of_childs; it++) {
                TSNode child = ts_node_child(node, it);
                ooo_print_nodes(child, row_begin, row_end, level);
        }
}
#endif

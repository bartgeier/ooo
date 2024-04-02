#include "node_printer.h"
#include "ooo_treesitter_symbol_ids.h"
#include <stdio.h>


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
                for (size_t i = 0; i <= level; i++) {
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

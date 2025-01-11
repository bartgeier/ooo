#include "gtest/gtest.h"
#include "tree_navigator.h"

#define size_of_arr(arr) sizeof((arr)) / sizeof(*(arr))

#if 0
TEST(tree_navigator, Nodes_init_) {
        Nodes nodes = Nodes_init(5);
        bool b = true;
        for (size_t i = 0; i < nodes.SIZE; i++) {
                TSNode node = Nodes_at(&nodes, i); 
                b &= (node.tree == NULL);
                b &= (node.id == NULL);
                for(size_t n = 0; n < size_of_arr(node.context); n++) {
                        b &= (node.context[n] == 0);
                }
        }
        EXPECT_TRUE(b);
        EXPECT_EQ(nodes.last, (size_t)0);
}

TEST(tree_navigator, Node_push) {
        Nodes nodes = Nodes_init(5);
        TSNode n;
        for (size_t i = 0; i < size_of_arr(n.context); i++) { 
                n.context[i] = 0; 
        }
        Nodes_push(&nodes, n);
        n.context[0] = 1;
        Nodes_push(&nodes, n);
        n.context[0] = 2;
        Nodes_push(&nodes, n);
        n.context[0] = 3;
        Nodes_push(&nodes, n);
        n.context[0] = 4;
        Nodes_push(&nodes, n);
        n.context[0] = 5;
        Nodes_push(&nodes, n);
        n.context[0] = 6;
        Nodes_push(&nodes, n);
        n.context[0] = 7;
        Nodes_push(&nodes, n);
        TSNode x;
        x = Nodes_at(&nodes, 0);
        EXPECT_EQ(x.context[0], (uint32_t)7);
        x = Nodes_at(&nodes, 1);
        EXPECT_EQ(x.context[0], (uint32_t)6);
        x = Nodes_at(&nodes, 2);
        EXPECT_EQ(x.context[0], (uint32_t)5);
        x = Nodes_at(&nodes, 3);
        EXPECT_EQ(x.context[0], (uint32_t)4);
        x = Nodes_at(&nodes, 4);
        EXPECT_EQ(x.context[0], (uint32_t)3);

        x = Nodes_at(&nodes, 1);
        EXPECT_EQ(x.context[0], (uint32_t)6);
}
#else
TEST(tree_navigator, Nodes_init_) {
        Relation relation = Relation_make(5);
        bool b = true;
        for (size_t i = 0; i < relation.tracks.SIZE; i++) {
                TSNode node = Relation_track_node(&relation, i);
                b &= (node.tree == NULL);
                b &= (node.id == NULL);
                for(size_t n = 0; n < size_of_arr(node.context); n++) {
                        b &= (node.context[n] == 0);
                }
        }
        EXPECT_TRUE(b);
        EXPECT_EQ(relation.tracks.last, (size_t)0);
}

TEST(tree_navigator, Node_push) {
        Relation relation = Relation_make(5);
        TSNode n;
        for (size_t i = 0; i < size_of_arr(n.context); i++) { 
                n.context[i] = 0; 
        }
        uint32_t child_idx = 0;
        Relation_track(&relation, n, child_idx);
        n.context[0] = 1;
        Relation_track(&relation, n, child_idx);
        n.context[0] = 2;
        Relation_track(&relation, n, child_idx);
        n.context[0] = 3;
        Relation_track(&relation, n, child_idx);
        n.context[0] = 4;
        Relation_track(&relation, n, child_idx);
        n.context[0] = 5;
        Relation_track(&relation, n, child_idx);
        n.context[0] = 6;
        Relation_track(&relation, n, child_idx);
        n.context[0] = 7;
        Relation_track(&relation, n, child_idx);
        TSNode x;
        x = Relation_track_node(&relation, 0);
        EXPECT_EQ(x.context[0], (uint32_t)7);
        x = Relation_track_node(&relation, 1);
        EXPECT_EQ(x.context[0], (uint32_t)6);
        x = Relation_track_node(&relation, 2);
        EXPECT_EQ(x.context[0], (uint32_t)5);
        x = Relation_track_node(&relation, 3);
        EXPECT_EQ(x.context[0], (uint32_t)4);
        x = Relation_track_node(&relation, 4);
        EXPECT_EQ(x.context[0], (uint32_t)3);

        x = Relation_track_node(&relation, 1);
        EXPECT_EQ(x.context[0], (uint32_t)6);
}
#endif



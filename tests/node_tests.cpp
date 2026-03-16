#include <gtest/gtest.h>
#include "grid/Node.h"

// TEST(TestSuiteName, TestName)
TEST(NodeTest, Initialization) {
    // Create a node at Grid X: 5, Grid Y: 10, Size: 20
    Node node(5, 10, 20.0f);

    // EXPECT_EQ checks if the left value EXACTLY equals the right value
    EXPECT_EQ(node.getGridX(), 5);
    EXPECT_EQ(node.getGridY(), 10);
    EXPECT_EQ(node.getType(), NodeType::Empty); // Nodes should start Empty
    EXPECT_EQ(node.parent, nullptr); // Breadcrumbs should start blank
}

TEST(NodeTest, StateChange) {
    Node node(0, 0, 20.0f);
    
    // Change the node to a solid wall
    node.setType(NodeType::Obstacle);
    
    // Verify the state actually changed in memory
    EXPECT_EQ(node.getType(), NodeType::Obstacle);
}
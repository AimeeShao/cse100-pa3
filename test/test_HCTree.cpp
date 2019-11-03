#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "HCTree.hpp"

using namespace std;
using namespace testing;

TEST(HCNodeTest, TEST_COMPARE_NODE_COUNT) {
    HCNode lhs(5, 'a');
    HCNode rhs(4, 'a');
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    pq.push(&lhs);
    pq.push(&rhs);
    // Assert rhs has higher priority due to lower count
    ASSERT_EQ(pq.top(), &rhs);
}

TEST(HCNodeTest, TEST_COMPARE_NODE_SYMBOL) {
    HCNode lhs(5, 'a');
    HCNode rhs(5, 'b');
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    pq.push(&lhs);
    pq.push(&rhs);
    // Assert rhs has higher priority due to higher ascii value
    ASSERT_EQ(pq.top(), &rhs);
}

TEST(HCTreeTest, TEST_BUILD_EMPTY) {
    HCTree tree;
    vector<unsigned int> freqs(256);
    tree.build(freqs);

    // Assert root and leaves are empty
    ASSERT_EQ(tree.getRoot(), nullptr);
    ASSERT_EQ(tree.getLeaves().size(), 0);
}

TEST(HCTreeTest, TEST_BUILD_SIMPLE) {
    HCTree tree;
    vector<unsigned int> freqs(256);
    freqs['a'] = 5;
    tree.build(freqs);

    // Assert root is HCNode(a, 5) and one leaf
    ASSERT_EQ(tree.getRoot()->count, 5);
    ASSERT_EQ(tree.getRoot()->symbol, 'a');

    ASSERT_EQ(tree.getRoot(), tree.getLeaves().at(0));
    ASSERT_EQ(tree.getLeaves().size(), 1);
}

/* Builds tree below:
 *       d
 *   c      d
 *  b  c  e  d
 * a b
 */
TEST(HCTreeTest, TEST_BUILD_LARGE) {
    HCTree tree;
    vector<unsigned int> freqs(256);
    freqs['a'] = 1;
    freqs['b'] = 2;
    freqs['c'] = 3;
    freqs['d'] = 5;
    freqs['e'] = 5;
    tree.build(freqs);

    // Assert root is HCNode(e, 16) and leaves
    ASSERT_EQ(tree.getRoot()->count, 16);
    ASSERT_EQ(tree.getRoot()->symbol, 'd');

    ASSERT_EQ(tree.getLeaves().size(), 5);
}

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['a'] = 2;
        freqs['b'] = 3;
        tree.build(freqs);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "0");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("1");
    ASSERT_EQ(tree.decode(is), 'b');
}

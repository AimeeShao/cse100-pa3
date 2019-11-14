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
    ASSERT_EQ(tree.getLeaves().size(), 256);
    ASSERT_EQ(tree.getLeaves().at(0), nullptr);
}

TEST(HCTreeTest, TEST_BUILD_SIMPLE) {
    HCTree tree;
    vector<unsigned int> freqs(256);
    freqs['a'] = 5;
    tree.build(freqs);

    ostringstream os;
    os << (*tree.getRoot());
    // Assert root is HCNode(a, 5) and one leaf
    ASSERT_EQ(os.str(), "[5,97]");
    ASSERT_EQ(tree.getRoot(), tree.getLeaves().at('a'));
}

TEST(HCTreeTest, TEST_BUILD_WITH_HEADER_SIMPLE) {
    string bitsStr = "10110000";
    string bitsStr2 = "10000000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2)) +
                   string(1, stoi(bitsStr2, nullptr, 2));

    stringstream ss;
    ss.str(ascii);
    BitInputStream bis(ss);

    HCTree tree;
    tree.buildWithHeader(bis, 1);
    // Assert we created a root and leaf of 'a'
    ASSERT_EQ(tree.getRoot()->symbol, 'a');
    ASSERT_EQ(tree.getRoot(), tree.getLeaves().at('a'));
}

TEST(HCTreeTest, TEST_BUILD_WITH_HEADER_LARGE) {
    HCTree tree;

    // 1c 1a 1b 0 0 1e 1d 0 0 padding
    // "101100011 101100001 101100010 0 0 101100101 101100100 0 0"
    string bitsStr = "10110001";
    string bitsStr2 = "11011000";
    string bitsStr3 = "01101100";
    string bitsStr4 = "01000101";
    string bitsStr5 = "10010110";
    string bitsStr6 = "11001000";
    string bitsStr7 = "01010101";

    string ascii = string(1, stoi(bitsStr, nullptr, 2)) +
                   string(1, stoi(bitsStr2, nullptr, 2)) +
                   string(1, stoi(bitsStr3, nullptr, 2)) +
                   string(1, stoi(bitsStr4, nullptr, 2)) +
                   string(1, stoi(bitsStr5, nullptr, 2)) +
                   string(1, stoi(bitsStr6, nullptr, 2)) +
                   string(1, stoi(bitsStr7, nullptr, 2));

    stringstream ss;
    ss.str(ascii);
    BitInputStream bis(ss);

    tree.buildWithHeader(bis, 5);

    // Assert we created correct tree with specific symbols
    ASSERT_EQ(tree.getRoot()->c0->c1->c1->symbol, 'b');
    ASSERT_EQ(tree.getRoot()->c1->c0->symbol, 'e');
}

TEST(HCTreeTest, TEST_ENCODE_SIMPLE) {
    HCTree tree;
    vector<unsigned int> freqs(256);
    freqs['a'] = 5;
    tree.build(freqs);

    istringstream is("0");
    // Assert decoding one node works
    ASSERT_EQ(tree.decode(is), 'a');
}

TEST(HCTreeTest, TEST_DECODE_SIMPLE) {
    HCTree tree;
    vector<unsigned int> freqs(256);
    freqs['a'] = 5;
    tree.build(freqs);

    ostringstream os;
    tree.encode('a', os);
    tree.encode('b', os);  // should encode nothing
    // Assert encode works for one node
    ASSERT_EQ(os.str(), "0");
}

TEST(HCTreeTest, TEST_DECODE_BIT_SIMPLE) {
    HCTree tree;
    vector<unsigned int> freqs(256);
    freqs['a'] = 5;
    tree.build(freqs);

    istringstream is("0");
    BitInputStream bis(is);

    // Assert correct decoding for one char bitstream
    ASSERT_EQ(tree.decode(bis), 'a');
}

TEST(HCTreeTest, TEST_BINARY_REP_SIMPLE) {
    HCTree tree;
    vector<unsigned int> freqs(256);
    freqs['a'] = 5;
    tree.build(freqs);

    vector<int> childrenCount;
    childrenCount.push_back('a');
    // Assert vector contains just 'a' for root
    ASSERT_EQ(childrenCount, tree.binaryRep());
}

/* Builds tree below:
 *         d
 *    b        d
 *  c   b     e  d
 *     a  b
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

    // Assert root is HCNode(e, 16)
    ASSERT_EQ(tree.getRoot()->count, 16);
    ASSERT_EQ(tree.getRoot()->symbol, 'd');
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
    // Assert correct encoding for 'a'
    ASSERT_EQ(os.str(), "0");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("1");
    // Assert correct decoding for 'b'
    ASSERT_EQ(tree.decode(is), 'b');
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODE_BITSTREAM) {
    ostringstream os;
    BitOutputStream bos(os);
    tree.encode('a', bos);
    // Assert encoding works
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE_BITSTREAM) {
    istringstream is("0");
    BitInputStream bis(is);

    // Assert correct decoding for bitstream
    ASSERT_EQ(tree.decode(bis), 'a');
}

class LargeHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    /* Builds tree below:
     *           d16
     *     b6         d10
     *  c3   b3     e5  d5
     *     a1  b2
     */
    LargeHCTreeFixture() {
        vector<unsigned int> freqs(256);
        freqs['a'] = 1;
        freqs['b'] = 2;
        freqs['c'] = 3;
        freqs['d'] = 5;
        freqs['e'] = 5;
        tree.build(freqs);
    }
};

TEST_F(LargeHCTreeFixture, TEST_ENCODE_LARGE) {
    ostringstream os;
    tree.encode('a', os);  // 010
    tree.encode('b', os);  // 011
    tree.encode('c', os);  // 00
    tree.encode('d', os);  // 11
    tree.encode('e', os);  // 10
    // Assert correct encoding for each symbol
    ASSERT_EQ(os.str(), "010011001110");
}

TEST_F(LargeHCTreeFixture, TEST_DECODE_LARGE) {
    // Assert correct decoding for a few symbols
    istringstream is("011");
    ASSERT_EQ(tree.decode(is), 'b');
    istringstream in("11");
    ASSERT_EQ(tree.decode(in), 'd');
}

TEST_F(LargeHCTreeFixture, TEST_BINARY_REP) {
    // Assert correct header for large tree
    vector<int> childrenCount;
    childrenCount.push_back('c');
    childrenCount.push_back('a');
    childrenCount.push_back('b');
    childrenCount.push_back(-1);
    childrenCount.push_back(-1);
    childrenCount.push_back('e');
    childrenCount.push_back('d');
    childrenCount.push_back(-1);
    childrenCount.push_back(-1);

    ASSERT_EQ(childrenCount, tree.binaryRep());
}
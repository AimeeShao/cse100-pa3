/**
 * An HCTree that stores HCNodes. Used to encode and decode bits from ostream
 * and istream using Huffman Coding algorithm. An HCNode has higher priority
 * if it has a lower count or if it has an equal count and higher ascii value.
 * HCNodes with higher priority is the c0 child if parent exists and the symbol
 * of parents are based on c1 child.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
#include "HCTree.hpp"
#include <stack>

#define BIT_IN_BYTE 8         // used for output symbol
#define BINARY 2               // binary is base 2

/* Deconstructor.
 * Deallocates the memory of the HCTree and all the HCNodes inside of it.
 */
HCTree::~HCTree() { deleteHCNodes(root); }

/* Builds the HCTree from a given frequency vector. Only non-zero frequencies
 * go in the tree.
 * @param freqs Frequency counts of ascii characters
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    // create priority queue for sorting nodes
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    for (unsigned int i = 0; i < freqs.size(); i++) {  // loop freqs, add leaves
        if (freqs.at(i) != 0) {  // only add symbols that don't have 0 freq
            HCNode* node = new HCNode(freqs.at(i), i);
            // add node to leaves vector and pq
            leaves[i] = node;
            pq.push(node);
        }
    }

    while (pq.size() > 1) {  // loop until we reach one root
        // get top two nodes (with lowest frequency or higher ascii)
        HCNode* leftNode = pq.top();
        pq.pop();
        HCNode* rightNode = pq.top();
        pq.pop();

        // create leftNode and rightNode's parent and assign respective pointers
        HCNode* parent =
            new HCNode(leftNode->count + rightNode->count, rightNode->symbol);
        leftNode->p = parent;
        rightNode->p = parent;
        parent->c0 = leftNode;
        parent->c1 = rightNode;

        // push parent into pq to be considered for root
        pq.push(parent);
    }

    // if there is a node, last node in pq is root node
    if (pq.size() == 1) {
        root = pq.top();
        pq.pop();
    }
}

/* Builds the HCTree by reading in bit by bit. 0 for internal node or 1 for leaf
 * node then reads in symbol.
 * @param inBit BitInputStream to read from
 * @param nonZeros Number of non zero freqs
 */
void HCTree::buildWithHeader(BitInputStream& inBit, unsigned int nonZeros) {
    unsigned int headerBit = 0;
    stack<HCNode*> nodes;  // stores nodes to build tree

    // keep reading bits until we read all symbols and down to last node = root
    while (nodes.size() > 1 || nonZeros != 0) {
        headerBit = inBit.readBit();  // read next bit

        if (headerBit == 0) {  // internal node, combine two nodes
            HCNode* c1 = nodes.top();
            nodes.pop();
            HCNode* c0 = nodes.top();
            nodes.pop();

            HCNode* parent = new HCNode(0, 0, c0, c1, 0);
            c0->p = parent;
            c1->p = parent;
            nodes.push(parent);
        } else {  // symbol, so create leaf node and push to nodes stack

            // recreate symbol from binary
            unsigned char symbol = 0;
            for (int i = 0; i < BIT_IN_BYTE; i++) {
                symbol *= BINARY;
                symbol += inBit.readBit();
            }
            HCNode* leaf = new HCNode(0, symbol, 0, 0, 0);
            nodes.push(leaf);

            // add to leaves vector for tree
            leaves[symbol] = leaf;
            nonZeros--;
        }
    }
    root = nodes.top();  // root is last node in nodes
    nodes.pop();
}

/* Writes the encoding bits of given symbol to given BitOutputStream.
 * @param symbol to encode into bits and to write to BitOutputStream
 * @param out BitOutputStream to write encoded bit to
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    HCNode* curr = leaves[symbol];  // node to keep track of encoding 0 or 1
    vector<unsigned int> encoding;  // encoding for symbol

    if (curr == nullptr) {  // symbol does not exist in tree
        return;
    }
    while (curr->p) {  // loop up the tree until we reach root
        // check if c0 or c1 child to add encoding
        if (curr->p->c0 == curr) {
            encoding.push_back(0);
        } else if (curr->p->c1 == curr) {
            encoding.push_back(1);
        }
        curr = curr->p;
    }

    // if no encoding, then theres only one leaf and encoding will just be 0
    if (encoding.size() == 0) {
        out.writeBit(0);
    } else {  // print out encoding bits in reverse order
        for (int i = encoding.size() - 1; i >= 0; i--) {
            out.writeBit(encoding[i]);
        }
    }
}

/* Writes the encoding bits of given symbol to ostream as 0 or 1.
 * @param symbol to encode into bits and to write to ostream
 * @param out ostream to write encoded bit to
 */
void HCTree::encode(byte symbol, ostream& out) const {
    HCNode* curr = leaves[symbol];  // node to keep track of encoding 0 or 1
    vector<unsigned int> encoding;  // encoding for symbol

    if (curr == nullptr) {  // symbol does not exist in tree
        return;
    }
    while (curr->p) {  // loop up the tree until we reach root
        // check if c0 or c1 child to add encoding
        if (curr->p->c0 == curr) {
            encoding.push_back(0);
        } else if (curr->p->c1 == curr) {
            encoding.push_back(1);
        }
        curr = curr->p;
    }

    // if no encoding, then theres only one leaf and encoding will just be 0
    if (encoding.size() == 0) {
        out << 0;
    } else {  // print out encoding in reverse order
        for (int i = encoding.size() - 1; i >= 0; i--) {
            out << encoding[i];
        }
    }
}

/* Decodes the sequence of bits from the BitInputStream and
 * returns the coded symbol.
 * @param in BitInputStream to take input bits from
 * @return byte that represents the decoded symbol of the inputted bit
 */
byte HCTree::decode(BitInputStream& in) const {
    HCNode* curr = root;  // stores where we are in the tree
    unsigned int bit;     // bit we read in

    while (curr) {  // read bits and traverse down the tree
        bit = in.readBit();
        if (bit == 0 && curr->c0) {  // go left (c0)
            curr = curr->c0;
        } else if (bit == 1 && curr->c1) {  // go right (c1)
            curr = curr->c1;
        }

        // return symbol if we reached end of tree
        if (curr->c0 == nullptr && curr->c1 == nullptr) {
            return curr->symbol;
        }
    }
    return 0;
}

/* Decodes the inputted bit (0,1) from the istream and returns the coded symbol.
 * @param in istream to take input bits from
 * @return byte that represents the decoded symbol of the inputted bit
 */
byte HCTree::decode(istream& in) const {
    HCNode* curr = root;  // stores where we are in the tree
    char bit;             // bit we read in

    while (curr && in.get(bit)) {      // read bits and traverse down the tree
        if (bit == '0' && curr->c0) {  // go left (c0)
            curr = curr->c0;
        } else if (bit == '1' && curr->c1) {  // go right (c1)
            curr = curr->c1;
        }

        // return symbol if we reached end of tree
        if (curr->c0 == nullptr && curr->c1 == nullptr) {
            return curr->symbol;
        }
    }
    return 0;
}

/* Used to create the header for the tree by doing post order traversal and
 * storing symbol if leaf node or -1 for internal node.
 * @return vector containing symbol of leaf or -1 for internal node
 */
vector<int> HCTree::binaryRep() const {
    vector<int> childrenCount;
    binaryRepRec(childrenCount, root);
    return childrenCount;
}

/* Helper for creating header of tree using recursion. Performs post order trav.
 * @param childrenCount Vector to store 1 or 0 for tree
 * @param curr Current node we are on
 */
void HCTree::binaryRepRec(vector<int>& childrenCount, HCNode* curr) const {
    // base case, curr == null, return
    if (curr == nullptr) {
        return;
    }

    binaryRepRec(childrenCount, curr->c0);
    binaryRepRec(childrenCount, curr->c1);

    // -1 for internal node or symbol for leaf node
    if (curr->c0 == nullptr && curr->c1 == nullptr) {
        childrenCount.push_back(curr->symbol);
    } else {
        childrenCount.push_back(-1);
    }
}

/* Helper for testing root node. Returns root node.
 * @return HCNode root
 */
HCNode* HCTree::getRoot() const { return root; }

/* Helper for testing leaves. Returns leaves vector.
 * @return leaves vector
 */
vector<HCNode*> HCTree::getLeaves() const { return leaves; }

/* Helper method for deleting all HCNodes.
 * @param node HCNode to delete subtree of and the node.
 */
void HCTree::deleteHCNodes(HCNode* node) {
    // base case
    if (node == nullptr) {
        return;
    }

    // delete children
    deleteHCNodes(node->c0);
    deleteHCNodes(node->c1);

    // then delete this node
    delete (node);
}
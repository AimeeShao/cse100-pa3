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

    for (int i = 0; i < freqs.size(); i++) {  // loop through freqs, add leaves
        if (freqs.at(i) != 0) {  // only add symbols that don't have 0 freq
            HCNode* node = new HCNode(freqs.at(i), i);
            // add node to leaves vector and pq
            leaves.push_back(node);
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

/* Writes the encoding bits of given symbol to given BitOutputStream.
 * @param symbol to encode into bits and to write to BitOutputStream
 * @param out BitOutputStream to write encoded bit to
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const { /*TODO*/
}

/* Writes the encoding bits of given symbol to ostream as 0 or 1.
 * @param symbol to encode into bits and to write to ostream
 * @param out ostream to write encoded bit to
 */
void HCTree::encode(byte symbol, ostream& out) const { /*TODO*/
}

/* Decodes the sequence of bits from the BitInputStream and
 * returns the coded symbol.
 * @param in BitInputStream to take input bits from
 * @return byte that represents the decoded symbol of the inputted bit
 */
byte HCTree::decode(BitInputStream& in) const { return ' '; /*TODO*/ }

/* Decodes the inputted bit (0,1) from the istream and returns the coded symbol.
 * @param in istream to take input bits from
 * @return byte that represents the decoded symbol of the inputted bit
 */
byte HCTree::decode(istream& in) const { return ' '; /*TODO*/ }

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
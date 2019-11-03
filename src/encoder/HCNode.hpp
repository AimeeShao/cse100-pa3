/**
 * HCNode class for implementing huffman's code. These HCNodes will make up the
 * elements in the HCTree. An HCNode node has higher priority if it has a lower
 * count or if it has equal count but higher ascii value. Higher priority is the
 * c0 child if exists.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
#ifndef HCNODE_HPP
#define HCNODE_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** A class, instances of which are nodes in an HCTree.
 */
class HCNode {
  public:
    unsigned int count;  // the freqency of the symbol
    byte symbol;         // byte in the file we're keeping track of
    HCNode* c0;          // pointer to '0' child
    HCNode* c1;          // pointer to '1' child
    HCNode* p;           // pointer to parent

    /* Constructor that initialize a HCNode */
    HCNode(unsigned int count, byte symbol, HCNode* c0 = 0, HCNode* c1 = 0,
           HCNode* p = 0)
        : count(count), symbol(symbol), c0(c0), c1(c1), p(p) {}
};

/* For printing an HCNode to an ostream. Possibly useful for debugging */
ostream& operator<<(ostream&, const HCNode&) __attribute__((weak));
ostream& operator<<(ostream& stm, const HCNode& n) {
    stm << "[" << n.count << "," << (int)(n.symbol) << "]";
    return stm;
}

/* Comparator of HCNode pointer. In priority queue, HCNode ptr with lower count
 * has higher priority, and if count is the same, HCNode ptr with larger symbol
 * has higher prioruty.
 */
struct HCNodePtrComp {
    /* The comparator or compare method for HCNode*s. HCNode with lower count
     * has higher priority. If count is same, HCNode with higher ascii value
     * has higher priority.
     * @param lhs Pointer to a HCNode to compare
     * @param rhs Pointer to a HCNode to compare
     * @return True if lhs has higher priority than rhs
     *  False if lhs has lower priority than rhs
     *  according to lower count or higher ascii value.*/
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        // compare count
        if (lhs->count < rhs->count) {  // lhs has higher priority
            return false;
        } else if (lhs->count > rhs->count) {  // rhs has higher priority
            return true;
        } else {  // counts are same, so compare ascii values of symbols
            if (lhs->symbol >
                rhs->symbol) {  // lhs has higher ascii and thus priority
                return false;
            } else {  // rhs has higher priority
                return true;
            }
        }
    }
};
#endif  // HCNODE_HPP

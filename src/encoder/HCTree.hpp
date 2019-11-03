/**
 * Header file of HCTree that stores HCNodes. Used to encode and decode bits
 * from ostream and istream using Huffman Coding algorithm. An HCNode has higher
 * priority if it has a lower count or if it has an equal count and higher ascii
 * value.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"

using namespace std;

/** TODO: class header */
class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes

  public:
    /* Explicit Constructor.
     * Initializes an empty HCTree */
    HCTree() { root = nullptr; }

    /* Deconstructor.
     * Deallocates the memory of the HCTree and all the HCNodes inside of it.
     */
    ~HCTree();

    /* Builds the HCTree from a given frequency vector. Only non-zero
     * frequencies go in the tree.
     * @param freqs Frequency counts
     */
    void build(const vector<unsigned int>& freqs);

    /* Writes the encoding bits of given symbol to given BitOutputStream.
     * @param symbol to encode into bits and to write to BitOutputStream
     * @param out BitOutputStream to write encoded bit to
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /* Writes the encoding bits of given symbol to ostream as 0 or 1.
     * @param symbol to encode into bits and to write to ostream
     * @param out ostream to write encoded bit to
     */
    void encode(byte symbol, ostream& out) const;

    /* Decodes the sequence of bits from the BitInputStream and
     * returns the coded symbol.
     * @param in BitInputStream to take input bits from
     * @return byte that represents the decoded symbol of the inputted bit
     */
    byte decode(BitInputStream& in) const;

    /* Decodes the inputted bit (0,1) from the istream and returns the coded
     * symbol.
     * @param in istream to take input bits from
     * @return byte that represents the decoded symbol of the inputted bit
     */
    byte decode(istream& in) const;

    /* Helper for testing root node. Returns root node.
     * @return HCNode root
     */
    HCNode* getRoot() const;

    /* Helper for testing leaves. Returns leaves vector.
     * @return leaves vector
     */
    vector<HCNode*> getLeaves() const;
};

#endif  // HCTREE_HPP

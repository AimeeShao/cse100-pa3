/**
 * An HCTree that stores HCNodes. Used to encode and decode bits from ostream
 * and istream using Huffman Coding algorithm. An HCNode has higher priority
 * if it has a lower count or if it has an equal count and higher ascii value.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
#include "HCTree.hpp"

/* Deconstructor.
 * Deallocates the memory of the HCTree and all the HCNodes inside of it.
 */
HCTree::~HCTree() { /* TODO */
}

/* Builds the HCTree from a given frequency vector. Only non-zero frequencies
 * go in the tree.
 * @param freqs Frequency counts
 */
void HCTree::build(const vector<unsigned int>& freqs) {}

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

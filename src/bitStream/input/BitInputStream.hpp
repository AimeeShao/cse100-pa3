/**
 * Input stream that takes in a byte but reads bits one by one.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>
typedef unsigned char byte;

using namespace std;

/** Class for BitInputStream that reads bits instead of the standard byte.
 *  Stores a byte into a buffer and reads bits one at a time.
 */
class BitInputStream {
  private:
    char buf;     // one byte buffer of bits
    int nbits;    // number of bits have been written to buf
    istream& in;  // reference to the input stream to use
    static const int BIT_IN_BYTE = 8;

  public:
    /* Constructor of BitInputStream.
     * Initializes values of buffer, nbits, and in stream.
     * @param is Reference to input stream to use
     */
    explicit BitInputStream(istream& is) : buf(0), nbits(BIT_IN_BYTE), in(is){};

    /* Fills the one byte buffer from the input stream. */
    void fill();

    /* Reads the next bit from the bit buffer. Fills buffer with next byte if
     * all the bits have been read.
     * @return 0 if bit read is 0, 1 if bit read is 1.*/
    unsigned int readBit();
};

#endif
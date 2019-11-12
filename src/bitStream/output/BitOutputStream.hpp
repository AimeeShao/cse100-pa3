/**
 * Output stream that writes bits one by one.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** Class for BitOutputStream that writes bits instead of the standard byte.
 *  Stores a byte into a buffer and writes bits one at a time.
 */
class BitOutputStream {
  private:
    char buf;      // one byte buffer of bits
    int nbits;     // number of bits have been writen to buf
    ostream& out;  // reference to the output stream to use
    static const int BIT_IN_BYTE = 8;

  public:
    /* Constructor of BitOutputStream.
     * Initializes values of buffer, nbits, and out stream.
     * @param out Reference to output stream to use
     */
    explicit BitOutputStream(ostream& os) : buf(0), nbits(0), out(os){};

    /* Sends buffer to output stream and clears buffer. */
    void flush();

    /* Writes least significant bit of given int to bit buffer. Flushes buffer
     * if full.
     * @param i Bit to write.
     */
    void writeBit(int i);
};

#endif

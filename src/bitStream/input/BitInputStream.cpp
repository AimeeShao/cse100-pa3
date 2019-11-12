/**
 * Input stream that takes in a byte but reads bits one by one.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
#include "BitInputStream.hpp"

/* Fills the one byte buffer from the input stream. */
void BitInputStream::fill() {
    unsigned char temp = in.get();  // get unsigned ascii byte
    buf = temp;                     // store into buffer
    nbits = 0;  // we read bits starting from index 0 of buffer
}

/* Reads the next bit from the bit buffer. Fills buffer with next byte if
 * all the bits have been read.
 * @return 0 if bit read is 0, 1 if bit read is 1.*/
unsigned int BitInputStream::readBit() {
    // fills buffer if all bits have been read
    if (nbits == BIT_IN_BYTE) {
        fill();
    }

    // get bit at specific nbits location - shift loc to index 7, get last digit
    unsigned char ubuf = buf;
    unsigned int bit = (ubuf >> (BIT_IN_BYTE - 1 - nbits)) & 1;

    // increment nbits
    nbits++;

    return bit;
}
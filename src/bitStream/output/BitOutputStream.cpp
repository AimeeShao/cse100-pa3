/**
 * Output stream that writes bits one by one.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
#include "BitOutputStream.hpp"

/* Sends buffer to output stream and clears buffer. */
void BitOutputStream::flush() {
    out << buf;  // write buffer to outstream
    buf = 0;     // clear buffer
    nbits = 0;   // reset nbits
}

/* Writes least significant bit of given int to bit buffer. Flushes buffer
 * if full.
 * @param i Bit to write.
 */
void BitOutputStream::writeBit(int i) {
    // write least significant bit to buffer at nbits
    buf = (i << (BIT_IN_BYTE - 1 - nbits)) | buf;

    // increment nbits
    nbits++;

    // check if buffer is full
    if (nbits == BIT_IN_BYTE) {
        flush();
    }
}

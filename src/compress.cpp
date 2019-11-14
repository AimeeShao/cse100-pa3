/**
 * Compresses a given file into a given output filename. Uses the Huffman
 * HCTree to compress characters into bits.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
#include <cmath>
#include <fstream>
#include <iostream>

#include "../subprojects/cxxopts/cxxopts.hpp"
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

#define TOTAL_SYMBOLS_BITS 32  // # of bits to represent total symbols
#define NON_ZEROS_BITS 9       // # of bits to represent nonZeros
#define BIT_IN_BYTE 8          // used for output symbol
#define BINARY 2               // binary is base 2
#define ASCII_MAX 256          // number of ascii values for HCTree

/* Perform pseudo compression with ascii encoding and naive header
 * (checkpoint). Read first file, build HCTree based on frequencies of each char
 * in first file, open compressed file, write header, then encoding.
 * @param inFileName File to read from
 * @param outFileName File to write compressed file to
 */
void pseudoCompression(string inFileName, string outFileName) {
    ifstream in(inFileName, ios::binary);  // open inFile

    HCTree tree;                            // HCTree to build and help encode
    vector<unsigned int> freqs(ASCII_MAX);  // stores freqs from input file

    char ch;  // stores character we are reading

    while (in.get(ch)) {  // get 1 character each time until no more
        unsigned char uch = ch;
        freqs[uch]++;
    }

    tree.build(freqs);  // build tree

    ofstream out(outFileName, ios::binary);  // open outFile
    for (unsigned int freq : freqs) {        // output header
        out << freq << endl;
    }

    in = ifstream(inFileName, ios::binary);  // reopen inFile
    while (in.get(ch)) {                     // get 1 character and encode
        unsigned char uch = ch;
        tree.encode(uch, out);  // output encoding
    }

    // close files
    in.close();
    out.close();
}

/* True compression with bitwise i/o and small header (final).
 * @param inFileName File to read from
 * @param outFileName File to write compressed file to
 * */
void trueCompression(string inFileName, string outFileName) {
    ifstream in(inFileName, ios::binary);  // open inFile

    HCTree tree;                            // HCTree to build and help encode
    vector<unsigned int> freqs(ASCII_MAX);  // stores freqs from input file
    unsigned int nonZeros = 0;              // number of nonZero freqs
    unsigned int totalSymbols = 0;          // number of symbols in input file

    char ch;  // stores character we are reading

    while (in.get(ch)) {  // get 1 character each time until no more
        unsigned char uch = ch;

        if (freqs[uch] == 0) {  // first time reading symbol
            nonZeros++;
        }
        freqs[uch]++;
    }

    // get number of total symbols
    for (unsigned int i = 0; i < freqs.size(); i++) {
        totalSymbols += freqs[i];
    }
    tree.build(freqs);  // build tree

    ofstream out(outFileName, ios::binary);  // open outFile
    BitOutputStream outBit(out);             // Bit output stream

    vector<int> childrenCount = tree.binaryRep();  // get tree rep for header
    // used to get binary for totalSymbols
    vector<unsigned int> totalSymbolBinary(TOTAL_SYMBOLS_BITS);
    vector<unsigned int> nonZerosBinary(NON_ZEROS_BITS);  // binary for nonZeros
    vector<unsigned int> symbolBinary(BIT_IN_BYTE);       // binary for symbols

    // convert totalSymbols to binary and output as part of header
    for (int i = TOTAL_SYMBOLS_BITS - 1; i >= 0; i--) {
        totalSymbolBinary[i] = totalSymbols % BINARY;
        totalSymbols /= BINARY;
    }
    for (int i = 0; i < TOTAL_SYMBOLS_BITS; i++) {
        outBit.writeBit(totalSymbolBinary[i]);
    }

    // convert nonZeros to binary and output as part of header
    for (int i = NON_ZEROS_BITS - 1; i >= 0; i--) {
        nonZerosBinary[i] = nonZeros % BINARY;
        nonZeros /= BINARY;
    }
    for (int i = 0; i < NON_ZEROS_BITS; i++) {
        outBit.writeBit(nonZerosBinary[i]);
    }

    // output rest of header
    for (unsigned int i = 0; i < childrenCount.size(); i++) {
        if (childrenCount[i] == -1) {  // internal node, output 0
            outBit.writeBit(0);
        } else {  // leaf, output 1, then symbol converted into binary
            outBit.writeBit(1);

            unsigned char count = childrenCount[i];
            // convert symbol to binary and output
            for (int j = BIT_IN_BYTE - 1; j >= 0; j--) {
                symbolBinary[j] = count % BINARY;
                count /= BINARY;
            }
            for (int j = 0; j < BIT_IN_BYTE; j++) {
                outBit.writeBit(symbolBinary[j]);
            }
        }
    }

    in = ifstream(inFileName, ios::binary);  // reopen inFile
    while (in.get(ch)) {                     // get 1 character and encode
        unsigned char uch = ch;
        tree.encode(uch, outBit);  // output encoding
    }

    // flush last bits stored in buffer
    outBit.flush();

    // close files
    in.close();
    out.close();
}

/* Main program that runs the compress. Checks if input file is invalid or
 * empty.
 * @param argc Number of arguments
 * @param argv Array of arguments
 */
int main(int argc, char* argv[]) {
    // option parsing for command line
    cxxopts::Options options("./compress",
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        exit(0);
    }
    // end option parsing

    FileUtils utils;  // utilities for checking files

    if (!utils.isValidFile(inFileName)) {  // invalid file
        return 0;
    } else if (utils.isEmptyFile(inFileName)) {  // empty file, create empty out
        ofstream out(outFileName, ios::binary);  // open outFile
        out.close();                             // close outFile
        return 0;
    }

    // No error, then compress
    if (isAsciiOutput) {
        pseudoCompression(inFileName, outFileName);
    } else {
        trueCompression(inFileName, outFileName);
    }

    return 0;
}
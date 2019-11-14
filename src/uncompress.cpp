/**
 * Uncompresses a given compressed file into a given output filename.
 * Rebuilds an HCTree using the compressed header to uncompress bits into
 * symbols.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
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

/* Perform pseudo decompression with ascii encoding and naive header
 * (checkpoint) Read compressed file, build HCTree based header, open
 * decompressed file, write decoding.
 * @param inFileName Compressed file to read from
 * @param outFileName File to write uncompressed file to
 */
void pseudoDecompression(string inFileName, string outFileName) {
    ifstream in(inFileName, ios::binary);  // open inFile

    HCTree tree;                            // HCTree to build and help decode
    vector<unsigned int> freqs(ASCII_MAX);  // stores freqs from input file

    char buf[BUFSIZ];        // Retrieve frequency of ascii values
    unsigned int index = 0;  // stores ascii value of char's freq we are reading

    while (index < ASCII_MAX) {  // get freq count for each ascii char
        in.getline(buf, BUFSIZ);
        unsigned int count = atoi(buf);  // convert buf to unsigned int for freq
        freqs[index] = count;
        index++;
    }

    unsigned int totalSymbols = 0;  // stores total symbols in file
    unsigned int symbolsRead = 0;   // how many symbols we read so far

    for (unsigned int i = 0; i < freqs.size(); i++) {  // gets totalSymbols
        totalSymbols += freqs[i];
    }

    tree.build(freqs);  // build tree

    ofstream out(outFileName, ios::binary);  // open outFile

    while (symbolsRead < totalSymbols) {  // outputs decoding for all symbols
        out << tree.decode(in);           // output decoded char
        symbolsRead++;
    }

    // close files
    in.close();
    out.close();
}

/* True decompression with bitwise i/o and small header (final)
 * @param inFileName Compressed file to read from
 * @param outFileName File to write uncompressed file to
 */
void trueDecompression(string inFileName, string outFileName) {
    ifstream in(inFileName, ios::binary);  // open inFile
    BitInputStream inBit(in);              // Bit input stream

    HCTree tree;                    // HCTree to build and help decode
    unsigned int totalSymbols = 0;  // stores total number of symbols to read
    unsigned int nonZeros = 0;      // stores nonZeros from header
    unsigned int symbolCount = 0;   // number of symbols read

    for (int i = 0; i < TOTAL_SYMBOLS_BITS; i++) {  // gets totalSymbols
        totalSymbols *= BINARY;
        totalSymbols += inBit.readBit();
    }

    for (int i = 0; i < NON_ZEROS_BITS; i++) {  // converts binary to nonZeros
        nonZeros *= BINARY;
        nonZeros += inBit.readBit();
    }

    tree.buildWithHeader(inBit, nonZeros);   // rebuild tree with rest of header
    ofstream out(outFileName, ios::binary);  // open outFile

    unsigned char decoding;
    while (symbolCount < totalSymbols) {  // decode until we read all symbols
        decoding = tree.decode(inBit);
        out << decoding;  // output decoded char
        symbolCount++;
    }

    // close files
    in.close();
    out.close();
}

/* Main program that runs the uncompress. Checks if input file is invalid or
 * empty.
 * @param argc Number of arguments
 * @param argv Array of arguments
 */
int main(int argc, char* argv[]) {
    // option parsing for command line
    cxxopts::Options options("./uncompress",
                             "Uncompresses files using Huffman Encoding");
    options.positional_help(
        "./path_to_compressed_input_file ./path_to_output_file");

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
    }

    // No error, then compress
    if (isAsciiOutput) {
        pseudoDecompression(inFileName, outFileName);
    } else {
        trueDecompression(inFileName, outFileName);
    }

    return 0;
}

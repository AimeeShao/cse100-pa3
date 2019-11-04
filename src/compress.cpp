/**
 * Compresses a given file into a given output filename. Uses the Huffman
 * HCTree to compress characters into bits.
 *
 * Author: Aimee T Shao
 * PID: A15444996
 */
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/* Perform pseudo compression with ascii encoding and naive header
 * (checkpoint). Read first file, build HCTree based on frequencies of each char
 * in first file, open compressed file, write header, then encoding.
 * @param inFileName File to read from
 * @param outFileName File to write compressed file to */
void pseudoCompression(string inFileName, string outFileName) {
    ifstream in(inFileName, ios::binary);  // open inFile

    HCTree tree;                            // HCTree to build and help encode
    const unsigned int ASCII_MAX = 256;     // number of ascii values for HCTree
    vector<unsigned int> freqs(ASCII_MAX);  // stores freqs from input file

    char ch;  // stores character we are reading

    while (in.get(ch)) {  // get 1 character each time until no more
        freqs[ch]++;
    }

    tree.build(freqs);  // build tree

    ofstream out(outFileName, ios::binary);  // open outFile
    for (unsigned int freq : freqs) {        // output header
        out << freq << endl;
    }

    in = ifstream(inFileName, ios::binary);  // reopen inFile
    while (in.get(ch)) {  // get 1 character and encode until no more to encode
        tree.encode(ch, out);  // output encoding
    }

    // close files
    in.close();
    out.close();
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(string inFileName, string outFileName) {}

/* Main program that runs the compress. Checks if input file is invalid or
 * empty.
 * @param argc Number of arguments
 * @param argv Array of arguments
 */
int main(int argc, char* argv[]) {
    FileUtils utils;  // utilities for checking files
    const char* inFileName = (argc >= 2) ? argv[1] : "";
    const char* outFileName = (argc >= 3) ? argv[2] : "";

    const char* usage =  // usage statement
        "Compresses files using Huffman Encoding\n"
        "Usage:\n"
        "  ./compress [OPTION...] ./path_to_input_file "
        "./path_to_output_file\n\n"
        "      --ascii\tWrite output in ascii mode instead of bit "
        "stream\n"
        "  -h, --help\tPrint help and exit\n";

    if (!utils.isValidFile(inFileName)) {  // print error and usage
        cout << usage << endl;
        return 0;
    }

    // No error, then compress
    pseudoCompression(inFileName, outFileName);

    return 0;
}
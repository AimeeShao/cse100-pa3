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

/* Perform pseudo decompression with ascii encoding and naive header
 * (checkpoint) Read compressed file, build HCTree based header, open
 * decompressed file, write decoding.
 * @param inFileName Compressed file to read from
 * @param outFileName File to write uncompressed file to
 */
void pseudoDecompression(string inFileName, string outFileName) {
    ifstream in(inFileName, ios::binary);  // open inFile

    HCTree tree;                            // HCTree to build and help decode
    const unsigned int ASCII_MAX = 256;     // number of ascii values for HCTree
    vector<unsigned int> freqs(ASCII_MAX);  // stores freqs from input file

    char buf[BUFSIZ];        // Retrieve frequency of ascii values
    unsigned int index = 0;  // stores ascii value of char's freq we are reading

    while (index < ASCII_MAX) {  // get freq count for each ascii char
        in.getline(buf, BUFSIZ);
        unsigned int count = atoi(buf);  // convert buf to unsigned int for freq
        freqs[index] = count;
        index++;
    }

    tree.build(freqs);  // build tree

    ofstream out(outFileName, ios::binary);  // open outFile

    unsigned char decoding;
    while ((decoding = tree.decode(in))) {
        out << decoding;  // output encoding
    }

    // close files
    in.close();
    out.close();
}

/* TODO: True decompression with bitwise i/o and small header (final) */
void trueDecompression(string inFileName, string outFileName) {}

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

#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include "BitOutputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitOutputStreamTests, SIMPLE_TEST) {
    stringstream ss;
    BitOutputStream bos(ss);
    bos.writeBit(1);
    bos.flush();

    // assert write bit less than max bits works correctly
    string bitsStr = "10000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}

TEST(BitOutputStreamTests, FLUSH_TEST) {
    stringstream ss;
    BitOutputStream bos(ss);
    for (int i = 0; i < 8; i++) {
        bos.writeBit(1);
    }
    bos.writeBit(0);
    bos.flush();

    // Assert flushing when reach max bits in buffer works
    string bitsStr = "11111111";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}
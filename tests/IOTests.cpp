#include <gtest/gtest.h>
#include <stdexcept>
#include "../src/io_lib.h"

TEST(IOString, IOString) {
    std::string s = "why we still here?";
    IOString io = IOString{ s };
    EXPECT_FALSE(io.eof());
    EXPECT_FALSE(io.is_closed());
    io.close();
    EXPECT_TRUE(io.is_closed());
}

TEST(StringReader, FailToReadInteger) {
    std::string s = "why we still 4ere?";
    StringReader r = StringReader{ s }; 
    EXPECT_THROW(r.read_int(), std::invalid_argument);
}

TEST(StringReader, readInt) {
    std::string s = "42 142    -11  +7abc";
    StringReader r = StringReader{ s }; 
    EXPECT_EQ(42, r.read_int());
    EXPECT_EQ(142, r.read_int());
    EXPECT_EQ(-11, r.read_int());
    EXPECT_EQ(7, r.read_int());
    EXPECT_THROW(r.read_int(), std::invalid_argument);
}

TEST(StringReader, tryToReadLongIntoInt) {
    std::string s = "18446744073709551615";
    StringReader r = StringReader { s };
    EXPECT_THROW(r.read_int(), std::out_of_range);
}

TEST(StringReader, eof) {
    std::string s = "";   
    StringReader r = StringReader{ s };
    EXPECT_TRUE(r.eof());
    EXPECT_THROW(r.read_int(), std::invalid_argument);
}

TEST(StringReader, readChar) {
    std::string s = "1ch";
    StringReader r = StringReader { s };

    EXPECT_EQ('1', r.read_char());
    EXPECT_EQ('c', r.read_char());
    EXPECT_EQ('h', r.read_char());
    EXPECT_TRUE(r.eof());
    // EXPECT_THROW(r.read_char(), std::invalid_argument);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}


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
    std::string s = "223372036854775807";
    StringReader r = StringReader { s };
    EXPECT_THROW(r.read_int(), std::out_of_range);
}

TEST(StringReader, ReadLongLong) {
    std::string s = "223372036854775807";
    StringReader r = StringReader { s };
    EXPECT_EQ(223372036854775807, r.read_llong());
}

TEST(StringReader, ReadDouble) {
    std::string s = "182.2";
    StringReader r = StringReader { s };
    EXPECT_EQ(182.2, r.read_double());
}

TEST(StringReader, TrickyDouble) {
    std::string s = "182.2.3.1415";
    StringReader r = StringReader { s };
    EXPECT_EQ(182.2, r.read_double());
    EXPECT_THROW(r.read_double(), std::invalid_argument);
    EXPECT_EQ('.', r.read_char());
    EXPECT_EQ(3.1415, r.read_double());
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
    EXPECT_THROW(r.read_char(), std::invalid_argument);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}


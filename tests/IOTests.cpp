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

TEST(StringReader, readWord) {
    std::string s = "1ch";
    StringReader r = StringReader { s };
    std::string ans;  
    r.read_word(ans);
    EXPECT_EQ(s, ans);
    EXPECT_TRUE(r.eof());
    EXPECT_THROW(r.read_word(ans), std::invalid_argument);
}

TEST(StringWriter, writeInt) {
    std::string s;
    StringWriter w = StringWriter(s);
    w.write_int(42);
    EXPECT_EQ("42", s);
}

TEST(StringWriter, writeString) {
    std::string s;
    StringWriter w = StringWriter(s);
    w.write_string("why we still here?");
    EXPECT_EQ("why we still here?", s);
}

TEST(StringReaderWriter, Uh) {
    std::string s;
    std::string res;
    StringReaderWriter uh = StringReaderWriter{s};
    uh.write_int(42);
    uh.write_string(" sh");
    EXPECT_EQ(42, uh.read_int());
}

IOFile::IOFile(const std::string& file_name) : file_name_ { file_name } {
    file_ = fopen(file_name_.c_str(), "r");
    if (file_ == nullptr) {
        throw std::invalid_argument(" Can't open file " + file_name_);
    }
}

void IOFile::close() {
    if (fclose(file_) != 0) {
        throw std::invalid_argument("ERROR: trouble in closing file " + file_name_);
    }
    is_closed_ = true;
}

bool IOFile::is_closed() const {
    return is_closed_;
}

bool IOFile::eof() const {
    return feof(file_);
}

IOFileReader::IOFileReader(const std::string& origin) : IOFile(origin) {};

char IOFileReader::read_char() {
    return getc(file_);    
}

int IOFileReader::read_int() {
    int res;
    fscanf(file_, "%d", &res);
    return res;
}

long IOFileReader::read_long() {
    long res;
    fscanf(file_, "%ld", &res);
    return res;
}

long long IOFileReader::read_llong() {
    long long res;
    fscanf(file_, "%lld", &res);
    return res;
}

double IOFileReader::read_double() {
    double res;    
    fscanf(file_, "%lf", &res);
    return res;
}

void IOFileReader::read_word(std::string& to) {
    char* str;
    fscanf(file_, "%s", str);
    to.append(str);
}

void IOFileWriter::write_char(char val) {
    fprintf(file_, "%c", val);
}

void IOFileWriter::write_int(int val) {
    fprintf(file_, "%d", val);
}

void IOFileWriter::write_string(const std::string& val) {
    fprintf(file_, "%s", val.c_str());
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}


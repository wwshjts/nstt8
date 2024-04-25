#pragma once
#include <string>
#include <optional> 
#include <cctype>
#include <stdexcept>
#include <iostream> 

class IO {
public:
    virtual void close() = 0;
    virtual bool is_closed() const = 0;
    virtual bool eof() const = 0;
};

class Reader : virtual public IO {
public:    
    virtual char read_char() = 0;
    virtual int read_int() = 0;
    virtual long read_long() = 0;
    virtual long long read_llong() = 0;
    virtual void read_word(std::string& dst) = 0;
};

class Writer : virtual public IO {
public:
    virtual void write_char(char val) = 0;
    virtual void write_int(int val) = 0;
    virtual void write_string(const std::string &val) = 0;
};

class IOString : virtual public IO {
protected:
    std::string& origin_;
    bool is_closed_ = false;
    size_t pos_ = 0;

public:
    IOString(std::string& ptr_to_string);
    void close();
    bool eof() const;
    bool is_closed() const;
};

class StringReader : virtual public IOString, virtual public Reader {
    std::optional<std::pair<std::string,size_t>> find_integer();
    std::optional<std::pair<std::string,size_t>> find_double();

public:
    StringReader(std::string& origin);
    ~StringReader() = default;
    char read_char();
    int read_int();
    long read_long();
    long long read_llong();
    double read_double();
    void read_word(std::string& to);
};

class StringWriter : virtual public IOString, virtual public Writer {

public:
    StringWriter(std::string& dst);
    ~StringWriter() = default;
    void write_char(char val);
    void write_int(int val);
    void write_string(const std::string &val);
};

class StringReaderWriter : virtual public StringWriter, virtual public StringReader {
public:
    StringReaderWriter(std::string& str) : StringWriter(str), StringReader(str), IOString(str) {}
};


class IOFile : virtual public IO {
protected:
    const std::string& file_name_;
    FILE* file_;
    bool is_closed_ = false;
    size_t pos_ = 0;

public:
    IOFile(const std::string& file_name);
    void close();
    bool eof() const;
    bool is_closed() const;
};

class IOFileReader : virtual public IOFile, virtual public Reader {
public:
    IOFileReader(const std::string& origin);
    char read_char();
    int read_int();
    long read_long();
    long long read_llong();
    double read_double();
    void read_word(std::string& to);
};

class IOFileWriter : virtual public IOFile, virtual public Writer {
public:
    IOFileWriter(const std::string& name) : IOFile{name} {};
    void write_char(char val);
    void write_int(int val);
    void write_string(const std::string &val);
};

class IOFileReaderWriter : virtual public IOFileWriter, virtual public IOFileReader {
    IOFileReaderWriter(const std::string& val) : IOFileReader(val), IOFileWriter(val), IOFile(val) {};  
};

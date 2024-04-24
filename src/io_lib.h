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
    // virtual long long read_llong() = 0;
    // virtual std::string read_word() = 0;
    // virtual std::string read_line() = 0;
};

class Writer : public IO {
public:
    virtual void write_char(char val) = 0;
    virtual void write_int(int val) = 0;
    virtual void write_string(const std::string &val) = 0;
    virtual void write_line(const std::string &val) = 0;
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

class StringReader : public IOString, public Reader {
    std::optional<std::pair<std::string,size_t>> find_integer();
    // std::optional<std::string> find_double();

public:
    StringReader(std::string& origin);
    char read_char();
    int read_int();
    long read_long();
};

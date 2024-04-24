#include "io_lib.h"
#include <optional>
#include <stdexcept>
#include <string>

IOString::IOString(std::string& ptr_to_string) : origin_ { ptr_to_string } {}

void IOString::close() {
    is_closed_ = true;  
}

bool IOString::is_closed() const {
    return is_closed_;
}

bool IOString::eof() const {
    return (pos_ >= origin_.size());
}

StringReader::StringReader(std::string& origin) : IOString { origin } {}

char StringReader::read_char() {
    if (eof()) {
        throw std::invalid_argument{ "EOF reached" };
    }
    return origin_[pos_++];
}

std::optional<std::pair<std::string, size_t>>StringReader::find_integer() {
    // skip spaces
    size_t spaces = 0;
    while (origin_[pos_ + spaces] == ' ') {
        spaces++;
    }

    size_t start = pos_ + spaces;
    size_t cnt = 0;
    bool sign = false;
    if ( (origin_[start] == '-') || (origin_[start] == '+')) {
        sign = true;
        cnt++;
    }

    while ((start + cnt < origin_.size()) && isdigit(origin_[start + cnt])) {
        cnt++;
    }

    if ((cnt == 1) && sign) {
        return std::nullopt;
    }
    
    return std::optional<std::pair<std::string, size_t>>{ std::pair<std::string, size_t>{origin_.substr(start, cnt), spaces + cnt}};
}

std::optional<std::pair<std::string, size_t>>StringReader::find_double() {
    // skip spaces
    size_t spaces = 0;
    while (origin_[pos_ + spaces] == ' ') {
        spaces++;
    }

    size_t start = pos_ + spaces;
    size_t cnt = 0;
    int dots = 0;
    bool sign = false;
    if ( (origin_[start] == '-') || (origin_[start] == '+')) {
        sign = true;
        cnt++;
    }

    if (origin_[start + cnt] == '.') {
        return std::nullopt;        
    }

    while ((start + cnt < origin_.size()) && (isdigit(origin_[start + cnt]) || (origin_[start + cnt] == '.'))) {
        if (origin_[start + cnt] == '.') {
            dots++;
        } 
        if (dots > 1) {
            break;
        }
        cnt++;
    }

    if ((cnt == 1) && sign) {
        return std::nullopt;
    }
    
    return std::optional<std::pair<std::string, size_t>>{ std::pair<std::string, size_t>{origin_.substr(start, cnt), spaces + cnt}};
}

int StringReader::read_int() {
    if (eof()) {
        throw std::invalid_argument{ "EOF reached" };
    }
    std::optional<std::pair<std::string, size_t>> finded = find_integer();
    if (!finded) {
        throw std::invalid_argument{ "Can't read integer from string" + std::to_string(pos_) };
    }
    pos_ += finded->second;
    return std::stoi(finded->first);
}

long StringReader::read_long() {
    if (eof()) {
        throw std::invalid_argument{ "EOF reached" };
    }
    std::optional<std::pair<std::string, size_t>> finded = find_integer();
    if (!finded) {
        throw std::invalid_argument{ "Can't read integer from string" + std::to_string(pos_) };
    }
    pos_ += finded->second;
    return std::stol(finded->first);
}

long long  StringReader::read_llong() {
    if (eof()) {
        throw std::invalid_argument{ "EOF reached" };
    }
    std::optional<std::pair<std::string, size_t>> finded = find_integer();
    if (!finded) {
        throw std::invalid_argument{ "Can't read integer from string" + std::to_string(pos_) };
    }
    pos_ += finded->second;
    return std::stoll(finded->first);
}

double StringReader::read_double() {
    if (eof()) {
        throw std::invalid_argument{ "EOF reached" };
    }
    
    std::optional<std::pair<std::string, size_t>> finded = find_double();
    if (!finded) {
        throw std::invalid_argument{ "Can't read integer from string" + std::to_string(pos_) };
    }
    pos_ += finded->second;
    return std::stod(finded->first);
}

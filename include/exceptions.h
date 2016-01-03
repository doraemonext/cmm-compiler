#ifndef CMM_EXCEPTIONS_H
#define CMM_EXCEPTIONS_H

#include <string>
#include <exception>
#include <sstream>
#include "utils.h"

// 词法解析器字符不匹配异常
class lexer_char_mismatch: public std::exception {
public:
    explicit lexer_char_mismatch(const LexerPosition &position, const std::string &msg) : position_(position), msg_(msg) {
        std::stringstream buffer;
        buffer << "Row " << position_.row() << " Col " << position_.col() << ": ";
        msg_ = buffer.str() + msg_;
    }

    virtual ~lexer_char_mismatch() throw () { }

    virtual const char *what() const throw() { return msg_.c_str(); }

protected:
    LexerPosition position_;
    std::string msg_;
};

#endif //CMM_EXCEPTIONS_H
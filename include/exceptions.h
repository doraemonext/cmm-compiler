#ifndef CMM_EXCEPTIONS_H
#define CMM_EXCEPTIONS_H

#include <string>
#include <exception>
#include <sstream>
#include "utils.h"

// 词法解析器字符不匹配异常
class lexer_exception : public std::exception {
public:
    explicit lexer_exception(const LexerPosition &position, const std::string &msg) : position_(position), msg_(msg) {
        std::stringstream buffer;
        buffer << "第 " << position_.row() << " 行 第 " << position_.col() << " 列: ";
        msg_ = buffer.str() + msg_;
    }

    virtual ~lexer_exception() throw() { }

    virtual const char *what() const throw() { return msg_.c_str(); }

protected:
    LexerPosition position_;
    std::string msg_;
};

// 语法解析器字符不匹配异常
class parser_exception : public std::exception {
public:
    explicit parser_exception(const TokenPosition &position, const std::string &msg) : position_(position), msg_(msg) {
        std::stringstream buffer;
        buffer << "第 " << position_.row() << " 行 第 " << position_.col() << " 列: ";
        msg_ = buffer.str() + msg_;
    }

    virtual ~parser_exception() throw() { }

    virtual const char *what() const throw() { return msg_.c_str(); }

protected:
    TokenPosition position_;
    std::string msg_;
};

class scope_not_found : public std::exception { };

class scope_name_exists : public std::exception { };

class scope_critical_error : public std::exception { };

#endif //CMM_EXCEPTIONS_H
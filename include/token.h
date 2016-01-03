#ifndef CMM_TOKEN_H
#define CMM_TOKEN_H

#include <string>
#include "utils.h"

// Token 枚举类型
enum TokenType {
    kEOF = 0,            // 文件结束

    kRead,               // read
    kWrite,              // write
    kWhile,              // while
    kIf,                 // if
    kElse,               // else

    kInt,                // int
    kReal,               // real
    kVoid,               // void
    kFunc,               // func
    kReturn,             // return

    kLeftParen,          // (
    kRightParen,         // )
    kLeftBracket,        // [
    kRightBracket,       // ]
    kLeftBrace,          // {
    kRightBrace,         // }
    kSemicolon,          // ;
    kComma,              // ,
    kAssign,             // =
    kPlus,               // +
    kMinus,              // -
    kTimes,              // *
    kDivide,             // /
    kLT,                 // <
    kLTE,                // <=
    kGT,                 // >
    kGTE,                // >=
    kEqual,              // ==
    kNotEqual,           // <>
    kLineComment,        // //
    kLeftBlockComment,   // /*
    kRightBlockComment,  // */

    kIdentity,           // ID
    kIntegerLiteral,     // int 类型
    kRealLiteral,        // real 类型
    kWhiteSpace,         // 空白字符类型
};

// Token 类
class Token {
public:
    Token(const TokenType &type, const std::string &content, const Position &position) :
            type_(type), content_(content), position_(position) { }

private:
    TokenType type_;        // Token 类型
    std::string content_;   // Token 内容
    Position position_;     // Token 所处位置
};

#endif //CMM_TOKEN_H

#ifndef CMM_TOKEN_H
#define CMM_TOKEN_H

#include <string>
#include "utils.h"

// Token 类
class Token {
public:
    // Token 枚举类型
    enum class LexerType {
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

        kNone,               // 空类型
    };

    enum class ParserType {
        kProgram = 0,

        kStatements,
        kFunctions,

        kStatement,
        kFunction,

        kIfStatement,
        kWhileStatement,
        kReadStatement,
        kWriteStatement,
        kAssignStatement,
        kDeclareStatement,
        kReturnStatement,

        kDeclareKeyword,
        kCondition,
        kExpression,
        kTerm,
        kComparisonOp,
        kAddOp,
        kMulOp,
        kFactor,
        kArray,

        kFunc,
        kReturn,
        kRead,
        kWrite,
        kWhile,
        kIf,
        kElse,
        kInt,
        kReal,
        kVoid,
        kSemicolon,
        kLeftParen,
        kRightParen,
        kLeftBracket,
        kRightBracket,
        kLeftBrace,
        kRightBrace,
        kComma,
        kAssign,
        kPlus,
        kMinus,
        kTimes,
        kDivide,
        kLT,
        kLTE,
        kGT,
        kGTE,
        kEqual,
        kNotEqual,
        kIdentity,
        kIntegerLiteral,
        kRealLiteral,

        kNone,
    };

public:
    Token() : type_(LexerType::kNone), content_(""), position_(TokenPosition()) { }

    Token(const LexerType &type, const std::string &content, const TokenPosition &position) : type_(type), content_(content), position_(position) { }

    Token(const LexerType &type, const TokenPosition &position) : type_(type), content_(""), position_(position) { }

    static const char *token_type_name(const LexerType &type);

    const char *type_name() const {
        return token_type_name(type_);
    }

    LexerType type() const {
        return type_;
    }

    void set_type(LexerType type) {
        type_ = type;
    }

    std::string content() const {
        if (content_.length() > 0) {
            return content_;
        } else {
            return type_name();
        }
    }

    void set_content(const std::string &content) {
        content_ = content;
    }

    TokenPosition position() const {
        return position_;
    }

    void set_position(const TokenPosition &position) {
        position_ = position;
    }

    friend std::ostream &operator << (std::ostream &os, const Token &token) {
        std::cout << "第 " << token.position().row() << " 行 第 " << token.position().col() << " 列: <\"" << token.type_name() << "\", \"" << token.content() << "\", " << token.position().length() << ">";
        return os;
    }

private:
    LexerType type_;             // Token 类型
    std::string content_;   // Token 内容
    TokenPosition position_;     // Token 所处位置
};

#endif //CMM_TOKEN_H

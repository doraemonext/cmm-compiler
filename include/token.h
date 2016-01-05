#ifndef CMM_TOKEN_H
#define CMM_TOKEN_H

#include <string>
#include "utils.h"

// Token 类
class Token {
public:
    // Token 枚举类型
    enum class Type {
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

        kProgram,            // 程序整体

        kStatements,         // 多语句
        kFunctions,          // 多函数

        kStatement,          // 语句
        kFunction,           // 函数
        kFunctionParameters, // 函数参数
        kFunctionStatements, // 函数体
        kFunctionReturnStatement,  // 函数返回语句

        kIfStatement,        // if 语句
        kWhileStatement,     // while 语句
        kReadStatement,      // read 语句
        kWriteStatement,     // write 语句
        kAssignStatement,    // 赋值语句
        kDeclareStatement,   // 定义语句
        kReturnStatement,    // 返回语句

        kDeclareKeyword,     // 定义关键字
        kCondition,          // 判别式
        kExpression,         // 表达式
        kTerm,               // 元素
        kComparisonOp,       // 比较符
        kAddOp,              // 加减比较符
        kMulOp,              // 乘除比较符
        kFactor,             // 元素
        kArray,              // 数组
        kFunctionCall,       // 函数调用

        kResult,             // 计算结果

        kNone,               // 空类型
    };

public:
    Token() : type_(Type::kNone), content_(""), position_(TokenPosition()) { }

    Token(const Type &type, const std::string &content, const TokenPosition &position) : type_(type), content_(content), position_(position) { }

    Token(const Type &type, const TokenPosition &position) : type_(type), content_(""), position_(position) { }

    static const char *token_type_name(const Type &type);

    const char *type_name() const {
        return token_type_name(type_);
    }

    Type type() const {
        return type_;
    }

    void set_type(Type type) {
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
        os << token.content();
        return os;
    }

    void print() const {
        std::cout << "第 " << position().row() << " 行 第 " << position().col() << " 列: <\"" << type_name() << "\", \"" << content() << "\", " << position().length() << ">";
    }

private:
    Type type_;             // Token 类型
    std::string content_;   // Token 内容
    TokenPosition position_;     // Token 所处位置
};

#endif //CMM_TOKEN_H

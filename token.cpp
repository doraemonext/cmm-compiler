#include "include/token.h"

const char *Token::token_type_name(const LexerType &type) {
    switch (type) {
        case LexerType::kEOF: return "EOF";
        case LexerType::kRead: return "read";
        case LexerType::kWrite: return "write";
        case LexerType::kWhile: return "while";
        case LexerType::kIf: return "if";
        case LexerType::kElse: return "else";
        case LexerType::kInt: return "int";
        case LexerType::kReal: return "real";
        case LexerType::kVoid: return "void";
        case LexerType::kFunc: return "func";
        case LexerType::kReturn: return "return";
        case LexerType::kLeftParen: return "(";
        case LexerType::kRightParen: return ")";
        case LexerType::kLeftBracket: return "[";
        case LexerType::kRightBracket: return "]";
        case LexerType::kLeftBrace: return "{";
        case LexerType::kRightBrace: return "}";
        case LexerType::kSemicolon: return ";";
        case LexerType::kComma: return ",";
        case LexerType::kAssign: return "=";
        case LexerType::kPlus: return "+";
        case LexerType::kMinus: return "-";
        case LexerType::kTimes: return "*";
        case LexerType::kDivide: return "/";
        case LexerType::kLT: return "<";
        case LexerType::kLTE: return "<=";
        case LexerType::kGT: return ">";
        case LexerType::kGTE: return ">=";
        case LexerType::kEqual: return "==";
        case LexerType::kNotEqual: return "<>";
        case LexerType::kLineComment: return "//";
        case LexerType::kLeftBlockComment: return "/*";
        case LexerType::kRightBlockComment: return "*/";
        case LexerType::kIdentity: return "identity";
        case LexerType::kIntegerLiteral: return "integer_literal";
        case LexerType::kRealLiteral: return "real_literal";
        case LexerType::kWhiteSpace: return "white_space";
        default: throw std::invalid_argument("received invalid type_ value");
    }
}

#include "include/token.h"

const char *Token::token_type_name(const Type &type) {
    switch (type) {
        case Type::kEOF: return "EOF";
        case Type::kRead: return "read";
        case Type::kWrite: return "write";
        case Type::kWhile: return "while";
        case Type::kIf: return "if";
        case Type::kElse: return "else";
        case Type::kInt: return "int";
        case Type::kReal: return "real";
        case Type::kVoid: return "void";
        case Type::kFunc: return "func";
        case Type::kReturn: return "return";
        case Type::kLeftParen: return "(";
        case Type::kRightParen: return ")";
        case Type::kLeftBracket: return "[";
        case Type::kRightBracket: return "]";
        case Type::kLeftBrace: return "{";
        case Type::kRightBrace: return "}";
        case Type::kSemicolon: return ";";
        case Type::kComma: return ",";
        case Type::kAssign: return "=";
        case Type::kPlus: return "+";
        case Type::kMinus: return "-";
        case Type::kTimes: return "*";
        case Type::kDivide: return "/";
        case Type::kLT: return "<";
        case Type::kLTE: return "<=";
        case Type::kGT: return ">";
        case Type::kGTE: return ">=";
        case Type::kEqual: return "==";
        case Type::kNotEqual: return "<>";
        case Type::kLineComment: return "//";
        case Type::kLeftBlockComment: return "/*";
        case Type::kRightBlockComment: return "*/";
        case Type::kIdentity: return "identity";
        case Type::kIntegerLiteral: return "integer_literal";
        case Type::kRealLiteral: return "real_literal";
        case Type::kWhiteSpace: return "white_space";
        default: throw std::invalid_argument("received invalid type_ value");
    }
}

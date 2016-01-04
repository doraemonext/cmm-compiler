#include "include/token.h"

const char *Token::token_type_name(const Type &type) {
    switch (type) {
        case kEOF: return "EOF";
        case kRead: return "read";
        case kWrite: return "write";
        case kWhile: return "while";
        case kIf: return "if";
        case kElse: return "else";
        case kInt: return "int";
        case kReal: return "real";
        case kVoid: return "void";
        case kFunc: return "func";
        case kReturn: return "return";
        case kLeftParen: return "(";
        case kRightParen: return ")";
        case kLeftBracket: return "[";
        case kRightBracket: return "]";
        case kLeftBrace: return "{";
        case kRightBrace: return "}";
        case kSemicolon: return ";";
        case kComma: return ",";
        case kAssign: return "=";
        case kPlus: return "+";
        case kMinus: return "-";
        case kTimes: return "*";
        case kDivide: return "/";
        case kLT: return "<";
        case kLTE: return "<=";
        case kGT: return ">";
        case kGTE: return ">=";
        case kEqual: return "==";
        case kNotEqual: return "<>";
        case kLineComment: return "//";
        case kLeftBlockComment: return "/*";
        case kRightBlockComment: return "*/";
        case kIdentity: return "identity";
        case kIntegerLiteral: return "integer_literal";
        case kRealLiteral: return "real_literal";
        case kWhiteSpace: return "white_space";
        default: throw std::invalid_argument("received invalid type_ value");
    }
}

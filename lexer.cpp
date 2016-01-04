#include "include/lexer.h"

Lexer::Lexer() : input_(""), index_(0), in_row_comment_(false), in_block_comment_(false) {
    current_ = npos;
}

Lexer::Lexer(const std::string &input) : input_(input), index_(0), in_row_comment_(false), in_block_comment_(false) {
    if (input.length() == 0) {
        current_ = npos;
    } else {
        current_ = input_[0];
        maintain_position();
    }
}

Lexer &Lexer::operator = (const Lexer &lexer) {
    input_ = lexer.input_;
    index_ = lexer.index_;
    current_ = lexer.current_;
    position_ = lexer.position_;
    in_row_comment_ = lexer.in_row_comment_;
    in_block_comment_ = lexer.in_block_comment_;
    return *this;
}

// 设置输入
void Lexer::set_input(std::string input) {
    input_ = input;
    index_ = 0;
    position_.clear();
    in_row_comment_ = false;
    in_block_comment_ = false;

    if (input.length() == 0) {
        current_ = npos;
    } else {
        current_ = input_[0];
        maintain_position();
    }
}

// 消费一个字符, 可以选择是否跳过空白字符
void Lexer::consume(const bool &skip = true) {
    advance();
    if (skip) {
        skip_whitespace();
    }
}

// 确认字符流中的下一个字符为 x, 否则报错
void Lexer::match(const char &x) {
    if (current_ == x) {
        consume();
    } else {
        std::stringstream buffer;
        buffer << "错误的字符 '" << current_ << "', 期望得到 '" << x << "'";
        throw lexer_exception(position_, buffer.str());
    }
}

// 获取当前字符流的下一个 Token
Token Lexer::next_token() {
    while (current_ != npos) {
        if (in_row_comment_) {  // 在行注释中
            while (current_ != npos && !Recognition::is_newline(current_)) {
                consume(false);
            }
            in_row_comment_ = false;
            continue;
        }
        if (in_block_comment_) {  // 在块注释中
            while (current_ != npos) {
                while (current_ != npos && current_ != '*') {
                    consume(false);
                }
                if (current_ == npos) {
                    throw lexer_exception(position_, "未闭合的注释块");
                } else {
                    Token t = match_times();
                    if (t.type() == Token::Type::kRightBlockComment) {
                        in_block_comment_ = false;
                        break;
                    }
                }
            }
            continue;  // 保证 EOF 会被正确处理
        }

        switch (current_) {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                skip_whitespace();
                continue;
            case '(': return match_left_paren();
            case ')': return match_right_paren();
            case '[': return match_left_bracket();
            case ']': return match_right_bracket();
            case '{': return match_left_brace();
            case '}': return match_right_brace();
            case ';': return match_semicolon();
            case ',': return match_comma();
            case '=': return match_equal();
            case '+': return match_plus();
            case '-': return match_minus();
            case '*': return match_times();
            case '/': return match_divide();
            case '<': return match_lt();
            case '>': return match_gt();
            default:
                if (Recognition::is_letter(current_)) {
                    return match_name();
                } else if (Recognition::is_digit(current_)) {
                    return match_digit();
                } else {
                    std::stringstream buffer;
                    buffer << "不合法的字符: " << current_;
                    throw lexer_exception(position_, buffer.str());
                }
        }
    }
    return Token(Token::Type::kEOF, TokenPosition(position_, 0));
}

// 前进一个字符
void Lexer::advance() {
    index_++;
    if (index_ >= input_.length()) {
        current_ = npos;
    } else {
        current_ = input_[index_];
    }
    maintain_position();
}

// 跳过空白字符
void Lexer::skip_whitespace() {
    while (current_ != npos && Recognition::is_whitespace(current_)) {
        advance();
    }
}

// 根据当前字符维护词法分析器的相对位置
void Lexer::maintain_position() {
    if (Recognition::is_newline(current_)) {
        position_.next_line();
    } else {
        position_.next_char();
    }
}

// 匹配 Token (
Token Lexer::match_left_paren() {
    consume();
    return Token(Token::Type::kLeftParen, TokenPosition(position_, 1));
}

// 匹配 Token )
Token Lexer::match_right_paren() {
    consume();
    return Token(Token::Type::kRightParen, TokenPosition(position_, 1));
}

// 匹配 Token [
Token Lexer::match_left_bracket() {
    consume();
    return Token(Token::Type::kLeftBracket, TokenPosition(position_, 1));
}

// 匹配 Token ]
Token Lexer::match_right_bracket() {
    consume();
    return Token(Token::Type::kRightBracket, TokenPosition(position_, 1));
}

// 匹配 Token {
Token Lexer::match_left_brace() {
    consume();
    return Token(Token::Type::kLeftBrace, TokenPosition(position_, 1));
}

// 匹配 Token }
Token Lexer::match_right_brace() {
    consume();
    return Token(Token::Type::kRightBrace, TokenPosition(position_, 1));
}

// 匹配 Token ;
Token Lexer::match_semicolon() {
    consume();
    return Token(Token::Type::kSemicolon, TokenPosition(position_, 1));
}

// 匹配 Token ,
Token Lexer::match_comma() {
    consume();
    return Token(Token::Type::kComma, TokenPosition(position_, 1));
}

// 匹配 Token = 和 ==
Token Lexer::match_equal() {
    consume(false);
    if (current_ == '=') {
        consume();
        return Token(Token::Type::kEqual, TokenPosition(position_, 2));
    } else {
        return Token(Token::Type::kAssign, TokenPosition(position_, 1));
    }
}

// 匹配 Token +
Token Lexer::match_plus() {
    consume();
    return Token(Token::Type::kPlus, TokenPosition(position_, 1));
}

// 匹配 Token -
Token Lexer::match_minus() {
    consume();
    return Token(Token::Type::kMinus, TokenPosition(position_, 1));
}

// 匹配 Token * 和 */
Token Lexer::match_times() {
    consume(false);
    if (current_ == '/') {
        consume();
        return Token(Token::Type::kRightBlockComment, TokenPosition(position_, 2));
    } else {
        return Token(Token::Type::kTimes, TokenPosition(position_, 1));
    }
}

// 匹配 Token / 和 // 和 /*
Token Lexer::match_divide() {
    consume(false);
    if (current_ == '/') {
        consume();
        in_row_comment_ = true;
        return Token(Token::Type::kLineComment, TokenPosition(position_, 2));
    } else if (current_ == '*') {
        consume();
        in_block_comment_ = true;
        return Token(Token::Type::kLeftBlockComment, TokenPosition(position_, 2));
    } else {
        return Token(Token::Type::kDivide, TokenPosition(position_, 1));
    }
}

// 匹配 Token < 和 <= 和 <>
Token Lexer::match_lt() {
    consume(false);
    if (current_ == '=') {
        consume();
        return Token(Token::Type::kLTE, TokenPosition(position_, 2));
    } else if (current_ == '>') {
        consume();
        return Token(Token::Type::kNotEqual, TokenPosition(position_, 2));
    } else {
        return Token(Token::Type::kLT, TokenPosition(position_, 1));
    }
}

// 匹配 Token > 和 >=
Token Lexer::match_gt() {
    consume(false);
    if (current_ == '=') {
        consume();
        return Token(Token::Type::kGTE, TokenPosition(position_, 2));
    } else {
        return Token(Token::Type::kGT, TokenPosition(position_, 1));
    }
}

// 匹配标识符和关键字
Token Lexer::match_name() {
    std::stringstream buffer;
    do {
        buffer << current_;
        consume(false);
    } while (Recognition::is_letter(current_) || Recognition::is_digit(current_));

    std::string res = buffer.str();
    int length = (int)res.length();
    if (res == "read") {
        return Token(Token::Type::kRead, TokenPosition(position_, length));
    } else if (res == "write") {
        return Token(Token::Type::kWrite, TokenPosition(position_, length));
    } else if (res == "while") {
        return Token(Token::Type::kWhile, TokenPosition(position_, length));
    } else if (res == "if") {
        return Token(Token::Type::kIf, TokenPosition(position_, length));
    } else if (res == "else") {
        return Token(Token::Type::kElse, TokenPosition(position_, length));
    } else if (res == "int") {
        return Token(Token::Type::kInt, TokenPosition(position_, length));
    } else if (res == "real") {
        return Token(Token::Type::kReal, TokenPosition(position_, length));
    } else if (res == "void") {
        return Token(Token::Type::kVoid, TokenPosition(position_, length));
    } else if (res == "func") {
        return Token(Token::Type::kFunc, TokenPosition(position_, length));
    } else if (res == "return") {
        return Token(Token::Type::kReturn, TokenPosition(position_, length));
    } else if (Recognition::is_id(res)) {
        return Token(Token::Type::kIdentity, res, TokenPosition(position_, length));
    } else {
        buffer.str("");
        buffer << "不正确的标识符: \"" << res << "\"";
        throw lexer_exception(position_, buffer.str());
    }
}

// 匹配数字
Token Lexer::match_digit() {
    std::stringstream buffer;
    do {
        buffer << current_;
        consume(false);
    } while (Recognition::is_digit(current_) || Recognition::is_dot(current_));

    std::string res = buffer.str();
    int length = (int)res.length();
    if (Recognition::is_integer(res)) {
        return Token(Token::Type::kIntegerLiteral, res, TokenPosition(position_, length));
    } else if (Recognition::is_real(res)) {
        return Token(Token::Type::kRealLiteral, res, TokenPosition(position_, length));
    } else {
        buffer.str("");
        buffer << "不正确的数字: \"" << res << "\"";
        throw lexer_exception(position_, buffer.str());
    }
}

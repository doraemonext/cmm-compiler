#ifndef CMM_LEXER_H
#define CMM_LEXER_H

#include <string>
#include "token.h"
#include "utils.h"
#include "exceptions.h"

// 词法解析器类
class Lexer {
public:
    const char npos = -1;

    Lexer(const std::string &input) : input_(input), index_(0) {
        if (input.length() == 0) {
            current_ = npos;
        } else {
            current_ = input_[0];
            maintain_position();
        }
    }

    // 消费一个字符, 可以选择是否跳过空白字符
    void consume(const bool &skip = true) {
        advance();
        if (skip) {
            while (current_ != npos && Recognition::is_whitespace(current_)) {
                advance();
            }
        }
    }

    // 前进一个字符
    void advance() {
        index_++;
        if (index_ >= input_.length()) {
            current_ = npos;
        } else {
            current_ = input_[index_];
        }
        maintain_position();
    }

    // 根据当前字符维护词法分析器的相对位置
    void maintain_position() {
        if (Recognition::is_newline(current_)) {
            position.next_line();
        } else {
            position.next_char();
        }
    }

    void match(const char &x) {
        if (current_ == x) {
            consume();
        } else {
            std::stringstream buffer;
            buffer << "错误的字符 '" << current_ << "', 期望得到 '" << x << "'";
            throw lexer_char_mismatch(position, buffer.str());
        }
    }

private:
    std::string input_;      // 输入字符文本
    int index_ = 0;          // 当前字符位置下标
    char current_;           // 当前字符
    LexerPosition position;  // 当前位置
};

#endif //CMM_LEXER_H


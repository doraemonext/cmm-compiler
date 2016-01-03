#ifndef CMM_LEXER_H
#define CMM_LEXER_H

#include <string>
#include "token.h"
#include "utils.h"
#include "exceptions.h"

// 词法解析器类
class Lexer {
public:
    const char npos = -1;  // 文件结束

    Lexer(const std::string &input);

    // 消费一个字符, 可以选择是否跳过空白字符
    void consume(const bool &skip);

    // 确认字符流中的下一个字符为 x, 否则报错
    void match(const char &x);

    // 获取当前字符流的下一个 Token
    Token next_token();

private:
    std::string input_;               // 输入字符文本
    int index_;                       // 当前字符位置下标
    char current_;                    // 当前字符
    LexerPosition position_;          // 当前位置
    bool in_row_comment_;             // 当前是否在行注释中
    bool in_block_comment_;           // 当前按是否在块注释中

    // 前进一个字符
    void advance();

    // 跳过空白字符
    void skip_whitespace();

    // 根据当前字符维护词法分析器的相对位置
    void maintain_position();

    // 匹配 Token (
    Token match_left_paren();

    // 匹配 Token )
    Token match_right_paren();

    // 匹配 Token [
    Token match_left_bracket();

    // 匹配 Token ]
    Token match_right_bracket();

    // 匹配 Token {
    Token match_left_brace();

    // 匹配 Token }
    Token match_right_brace();

    // 匹配 Token ;
    Token match_semicolon();

    // 匹配 Token ,
    Token match_comma();

    // 匹配 Token = 和 ==
    Token match_equal();

    // 匹配 Token +
    Token match_plus();

    // 匹配 Token -
    Token match_minus();

    // 匹配 Token * 和 */
    Token match_times();

    // 匹配 Token / 和 // 和 /*
    Token match_divide();

    // 匹配 Token < 和 <= 和 <>
    Token match_lt();

    // 匹配 Token > 和 >=
    Token match_gt();

    // 匹配标识符和关键字
    Token match_name();

    // 匹配数字
    Token match_digit();
};

#endif //CMM_LEXER_H


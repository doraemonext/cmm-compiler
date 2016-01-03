#ifndef CMM_UTILS_H
#define CMM_UTILS_H

#include <iostream>
#include <string>
#include <regex>

// 位置类
class Position {
public:
    // 默认初始位置为 第 1 行 第 0 列
    Position(int row = 1, int col = 0) : row_(row), col_(col) { }

    int row() const { return row_; }
    void set_row(int row) { row_ = row; }

    int col() const { return col_; }
    void set_col(int col) { col_ = col; }

    void inc_row() { row_++; }
    void inc_col() { col_++; }
    void reset_row() { row_ = 0; }
    void reset_col() { col_ = 0; }

    void next_line() {
        inc_row();
        reset_col();
    }

    void next_char() {
        inc_col();
    }

    friend std::ostream &operator << (std::ostream &os, const Position &position) {
        std::cout << "Row " << position.row() << " Col " << position.col();
        return os;
    }

private:
    int row_;  // 行
    int col_;  // 列
};

// 词法解析器位置类
class LexerPosition: public Position { };

// Token 位置类, 用于描述 Token 所在源文件的位置信息, 便于显示出错信息
class TokenPosition: public Position {
public:
    TokenPosition(int row, int col, int length) : Position(row, col), length_(length) { }

    int length() const { return length_; }
    void set_length(int length) { length_ = length; }

private:
    int length_;   // 长度
};

// 识别类, 用于判断指定字符或字符串或数字的类型及其是否合法
class Recognition {
public:
    // 识别字母
    static bool is_letter(const char &c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    // 识别数字
    static bool is_digit(const char &c) {
        return c >= '0' && c <= '9';
    }

    // 识别小数点
    static bool is_dot(const char &c) {
        return c == '.';
    }

    // 识别空白字符
    static bool is_whitespace(const char &c) {
        return c == ' ' || c == '\n' || c == '\t' || c == '\r';
    }

    // 识别换行
    static bool is_newline(const char &c) {
        return c == '\n';
    }

    // 识别整数
    static bool is_integer(const std::string &str) {
        try {
            std::string::size_type pos;
            std::cout << std::stoi(str, &pos) << std::endl;
            if (pos < str.length()) {
                throw std::invalid_argument("invalid int argument");
            }
            return true;
        } catch (const std::invalid_argument &e) {
            return false;
        } catch (const std::out_of_range &e) {
            return false;
        }
    }

    // 识别浮点数
    static bool is_real(const std::string &str) {
        try {
            std::string::size_type pos;
            std::stod(str, &pos);
            if (pos < str.length()) {
                throw std::invalid_argument("invalid real argument");
            }
            return true;
        } catch (const std::invalid_argument &e) {
            return false;
        } catch (const std::out_of_range &e) {
            return false;
        }
    }

    // 识别标识符
    static bool is_id(const std::string &str) {
        if (str.length() == 0) {
            throw std::invalid_argument("received empty string value");
        }
        return std::regex_match(str, std::regex("^\\w+$")) && str[str.length()-1] != '_' &&
                std::regex_match(str.substr(0, 1), std::regex("[A-Za-z]"));
    }
};

#endif //CMM_UTILS_H

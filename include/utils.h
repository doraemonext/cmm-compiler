#ifndef CMM_UTILS_H
#define CMM_UTILS_H

// 位置类, 用于描述 Token 所在源文件的位置信息, 便于显示出错信息
class Position {
public:
    Position(int row, int col, int length) : row_(row), col_(col), length_(length) { }

    int row() const { return row_; }
    void set_row(int row) { row_ = row; }

    int col() const { return col_; }
    void set_col(int col) { col_ = col; }

    int length() const { return length_; }
    void set_length(int length) { length_ = length; }

private:
    int row_;      // 行
    int col_;      // 列
    int length_;   // 长度
};

#endif //CMM_UTILS_H

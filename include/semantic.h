#ifndef CMM_SEMANTIC_H
#define CMM_SEMANTIC_H

#include <iostream>
#include <vector>
#include "ast.h"
#include "symbol.h"
#include "scope.h"
#include "ir.h"

class Semantic {
public:
    Semantic(AbstractSyntaxNode *root);

    void analyse();

    Token analyse_program();

    // 分析函数
    Token analyse_function(const int &pos);

    // 解析语句
    Token analyse_statement(const int &pos, const Symbol &function_symbol);

    // 解析 if 语句
    Token analyse_if_statement(const int &pos, const Symbol &function_symbol);

    // 解析 while 语句
    Token analyse_while_statement(const int &pos, const Symbol &function_symbol);

    // 解析 read 语句
    Token analyse_read_statement(const int &pos);

    // 解析 write 语句
    Token analyse_write_statement(const int &pos);

    // 解析赋值语句
    Token analyse_assign_statement(const int &pos);

    // 解析定义语句
    Token analyse_declare_statement(const int &pos);

    // 解析 return 语句
    Token analyse_return_statement(const int &pos, const Symbol &function_symbol);

    // 分析定义关键字
    Token analyse_declare_keyword(const int &pos);

    // 解析带数组偏移的标识符
    Token analyse_identity_array(const int &pos);

    // 解析判别式
    Token analyse_condition(const int &pos);

    // 解析关系运算符
    Token analyse_comparison_op(const int &pos);

    // 解析标识符
    Token analyse_identity(const int &pos);

    Token analyse_literal(const int &pos);

    // 解析表达式
    Token analyse_expression(const int &pos);

    // 解析算术式
    Token analyse_term(const int &pos);

    // 解析元素
    Token analyse_factor(const int &pos);

    Token analyse_add_op(const int &pos);

    Token analyse_mul_op(const int &pos);

    Token analyse_function_call(const int &pos);

    Token analyse_function_call_parameters(const int &pos, const Symbol &function_symbol);

    const IR &ir() const;

    void print_error_messages() const;

    void print_warning_messages() const;

    void print_ir() const;

private:
    AbstractSyntaxNode *root_;
    AbstractSyntaxNode *current_;
    ScopeTree tree_;
    IR ir_;
    int ir_indent_;
    std::vector<std::pair<Position, std::string> > error_messages_;
    std::vector<std::pair<Position, std::string> > warning_messages_;

    AbstractSyntaxNode *child(int offset);

    Token::Type child_type(int offset);

    int children_size();

    Token::Type current_token_type();

    void add_error_messages(const Position &position, const std::string &message);

    void add_warning_messages(const Position &position, const std::string &message);

    void build_function_ir_start(const Token &identity);

    void build_function_ir_args(const std::vector<std::pair<Token, Token> > &args);

    void build_function_ir_end(const Token &identity);

    void build_return_statement_ir(const Token &literal);

    void build_declare_statement_ir(const Token &declare_keyword, const Token &identity);

    void build_assign_statement_ir(const Token &left_identity, const Token &right_expression);

    void build_expression_ir(const Token &op);

    void build_term_ir(const Token &op);

    void build_function_call_ir(const Token &identity);

    void build_write_statement_ir();

    void build_read_statement_ir(const Token &token, const Symbol &symbol);

    void build_condition_ir(const Token &op);

    void build_while_statement_ir_begin(const std::string &signature);

    void build_while_statement_ir_jz(const std::string &signature);

    void build_while_statement_ir_end(const std::string &signature);

    void build_if_statement_ir_begin(const std::string &signature);

    void build_if_statement_ir_jz(const std::string &signature);

    void build_if_statement_ir_else(const std::string &signature);

    void build_if_statement_ir_end(const std::string &signature);
};

#endif //CMM_SEMANTIC_H


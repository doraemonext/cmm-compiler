#ifndef CMM_PARSER_H
#define CMM_PARSER_H

#include <vector>
#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "exceptions.h"

class Parser {
public:
    Parser(const Lexer &lexer, const int &total);

    // 获取下一个 Token 并移动当前位置
    void consume();

    // 获取向前看的第 step 个 Token
    const Token &forward_token(const int &step) const;

    // 尝试将当前的下一个 Token 与预期 Token 类型比对
    Token match(const Token::Type &x);

    // 获取 AST
    AbstractSyntaxNode *ast();

    // 解析整个程序
    void parse_program();

    // 解析函数
    // function: FUNC declare_keyword ID LPAREN (declare_keyword ID (COMMA declare_keyword ID)*)? RPAREN LBRACE statements RBRACE
    void parse_function();

    // 解析语句
    // statement: if_statement | while_statement | read_statement | write_statement | assign_statement | declare_statement | return_statement | SEMICOLON ;
    void parse_statement();

    // 解析 if 语句
    // if_statement: IF LPAREN condition RPAREN LBRACE statements RBRACE (ELSE LBRACE statements RBRACE)? ;
    void parse_if_statement();

    // 解析 while 语句
    // while_statement: WHILE LPAREN condition RPAREN LBRACE statements RBRACE ;
    void parse_while_statement();

    // 解析 read 语句
    // read_statement: READ LPAREN ID RPAREN SEMICOLON ;
    void parse_read_statement();

    // 解析 write 语句
    // write_statement: WRITE LPAREN expression RPAREN SEMICOLON ;
    void parse_write_statement();

    // 解析赋值语句
    // assign_statement: (ID | ID array) ASSIGN expression SEMICOLON ;
    void parse_assign_statement();

    // 解析返回语句
    // return_statement: RETURN (ID)? SEMICOLON ;
    void parse_return_statement();

    // 解析定义语句
    // declare_statement: declare_keyword ID(COMMA ID)* SEMICOLON ;
    void parse_declare_statement();

    // 解析定义关键字
    // declare_keyword: INT | INT array | REAL | REAL array | VOID ;
    void parse_declare_keyword();

    // 解析判别式
    // condition: expression comparison_op expression ;
    void parse_condition();

    // 解析表达式
    // expression: term (add_op term)* ;
    void parse_expression();

    // 解析元素
    // term: factor (mul_op factor)* ;
    void parse_term();

    // 解析关系运算符
    // comparison_op: LT | LTE | GT | GTE | EQUAL | NEQUAL ;
    void parse_comparison_op();

    // 解析加减法运算符
    // add_op: PLUS | MINUS ;
    void parse_add_op();

    // 解析乘除法运算符
    // mul_op: TIMES | DIVIDE ;
    void parse_mul_op();

    // 解析元素
    // factor: REAL_LITERAL | INTEGER_LITERAL | ID | LPAREN expression RPAREN | ID array | function_call ;
    void parse_factor();

    // 解析数组
    // array: LBRACKET (INTEGER_LITERAL | ID) RBRACKET ;
    void parse_array();

    // 解析函数调用
    // function_call: ID LPAREN ((ID | REAL_LITERAL | INTEGER_LITERAL) (COMMA (ID | REAL_LITERAL | INTEGER_LITERAL)*)?) RPAREN ;
    void parse_function_call();

    void print_ast() const;

private:
    Lexer lexer_;
    std::vector<Token> lookahead_;
    int total_;
    int index_;
    AbstractSyntaxNode *root_;
    AbstractSyntaxNode *current_;

    bool is_declare_keyword(const Token::Type &type);

    bool is_comparison_op(const Token::Type &type);

    bool is_add_op(const Token::Type &type);

    bool is_mul_op(const Token::Type &type);

    bool is_literal(const Token::Type &type);
};

#endif //CMM_PARSER_H

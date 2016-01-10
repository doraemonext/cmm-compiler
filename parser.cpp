#include "include/parser.h"

Parser::Parser(const Lexer &lexer, const int &total) {
    lexer_ = lexer;
    lookahead_.resize(5);
    total_ = total;
    index_ = 0;
    root_ = nullptr;
    current_ = nullptr;
    for (int i = 0; i < total_; i++) {
        consume();
    }
}

// 获取下一个 Token 并移动当前位置
void Parser::consume() {
    lookahead_[index_] = lexer_.next_token();
    while (lookahead_[index_].type() == Token::Type::kLineComment) {
        lookahead_[index_] = lexer_.next_token();
    }
    if (lookahead_[index_].type() == Token::Type::kLeftBlockComment) {
        lookahead_[index_] = lexer_.next_token();
        if (lookahead_[index_].type() == Token::Type::kRightBlockComment) {
            lookahead_[index_] = lexer_.next_token();
        } else {
            throw parser_exception(lookahead_[index_].position(), "未闭合的注释块");
        }
    }
    index_ = (index_ + 1) % total_;
}

// 获取向前看的第 step 个 Token
const Token &Parser::forward_token(const int &step = 1) const {
    return lookahead_[(index_ + step - 1) % total_];
}

// 尝试将当前的下一个 Token 与预期 Token 类型比对
Token Parser::match(const Token::Type &x) {
    Token token = forward_token();

    if (token.type() == x) {
        consume();
        return token;
    } else {
        std::stringstream buffer;
        buffer << "无效的标识符 \"" << token.content() << "\"";
        throw parser_exception(token.position(), buffer.str());
    }
}

// 获取 AST
AbstractSyntaxNode *Parser::ast() {
    return root_;
}

// 解析整个程序
void Parser::parse_program() {
    root_ = new AbstractSyntaxNode(Token(Token::Type::kProgram, TokenPosition(1, 0, 0)));
    current_ = root_;

    while (true) {
        if (forward_token().type() == Token::Type::kEOF) {
            break;
        }

        if (forward_token().type() == Token::Type::kFunc) {
            parse_function();
        } else {
            parse_statement();
        }
    }
}

// 解析函数
// function: FUNC declare_keyword ID LPAREN (declare_keyword ID (COMMA declare_keyword ID)*)? RPAREN LBRACE statements RBRACE
void Parser::parse_function() {
    current_ = current_->add_child(Token::Type::kFunction, forward_token());

    match(Token::Type::kFunc);
    parse_declare_keyword();
    current_->add_child(forward_token());
    match(Token::Type::kIdentity);

    // 解析函数参数
    match(Token::Type::kLeftParen);
    current_ = current_->add_child(Token::Type::kFunctionParameters, forward_token());
    if (forward_token().type() != Token::Type::kRightParen) {

        // 解析第一个参数
        current_ = current_->add_child(Token::Type::kFunctionParameter, forward_token());
        parse_declare_keyword();
        current_->add_child(forward_token());
        match(Token::Type::kIdentity);
        current_ = current_->parent();

        // 解析第二个及以后的函数参数
        while (true) {
            if (forward_token().type() != Token::Type::kComma) {
                break;
            }
            match(Token::Type::kComma);

            current_ = current_->add_child(Token::Type::kFunctionParameter, forward_token());
            parse_declare_keyword();
            current_->add_child(forward_token());
            match(Token::Type::kIdentity);
            current_ = current_->parent();
        }
    }
    current_ = current_->parent();
    match(Token::Type::kRightParen);

    // 解析函数体
    match(Token::Type::kLeftBrace);
    current_ = current_->add_child(Token::Type::kFunctionStatements, forward_token());
    while (true) {
        if (forward_token(1).type() == Token::Type::kRightBrace) {
            break;
        }
        parse_statement();
    }
    current_ = current_->parent();
    match(Token::Type::kRightBrace);

    current_ = current_->parent();
}

// 解析语句
// statement: if_statement | while_statement | read_statement | write_statement | assign_statement | declare_statement | return_statement | SEMICOLON ;
void Parser::parse_statement() {
    current_ = current_->add_child(Token::Type::kStatement, forward_token());
    std::stringstream buffer;

    if (forward_token().type() == Token::Type::kIf) {
        parse_if_statement();
    } else if (forward_token().type() == Token::Type::kWhile) {
        parse_while_statement();
    } else if (forward_token().type() == Token::Type::kRead) {
        parse_read_statement();
    } else if (forward_token().type() == Token::Type::kWrite) {
        parse_write_statement();
    } else if (forward_token().type() == Token::Type::kIdentity && forward_token(2).type() != Token::Type::kLeftParen) {
        parse_assign_statement();
    } else if (forward_token().type() == Token::Type::kIdentity && forward_token(2).type() == Token::Type::kLeftParen) {
        parse_function_call();
        match(Token::Type::kSemicolon);
    } else if (forward_token().type() == Token::Type::kReturn) {
        parse_return_statement();
    } else if (is_declare_keyword(forward_token().type())) {
        parse_declare_statement();
    } else if (forward_token().type() == Token::Type::kSemicolon) {
        match(Token::Type::kSemicolon);
    } else {
        buffer << "无效的标识符 \"" << forward_token().content() << "\", 无法解析该语句, 请确认是否合法";
        throw parser_exception(forward_token().position(), buffer.str());
    }

    current_ = current_->parent();
}

// 解析 if 语句
// if_statement: IF LPAREN condition RPAREN LBRACE statements RBRACE (ELSE LBRACE statements RBRACE)? ;
void Parser::parse_if_statement() {
    current_ = current_->add_child(Token::Type::kIfStatement, forward_token());
    std::stringstream buffer;

    match(Token::Type::kIf);
    match(Token::Type::kLeftParen);
    parse_condition();
    match(Token::Type::kRightParen);
    match(Token::Type::kLeftBrace);
    current_ = current_->add_child(Token::Type::kStatements, forward_token());
    while (forward_token().type() != Token::Type::kRightBrace) {
        parse_statement();
    }
    match(Token::Type::kRightBrace);
    current_ = current_->parent();

    if (forward_token().type() == Token::Type::kElse) {
        match(Token::Type::kElse);
        current_ = current_->add_child(Token::Type::kStatements, forward_token());
        match(Token::Type::kLeftBrace);
        while (forward_token().type() != Token::Type::kRightBrace) {
            parse_statement();
        }
        match(Token::Type::kRightBrace);
        current_ = current_->parent();
    }

    current_ = current_->parent();
}

// 解析 while 语句
// while_statement: WHILE LPAREN condition RPAREN LBRACE statements RBRACE ;
void Parser::parse_while_statement() {
    current_ = current_->add_child(Token::Type::kWhileStatement, forward_token());
    std::stringstream buffer;

    match(Token::Type::kWhile);
    match(Token::Type::kLeftParen);
    parse_condition();
    match(Token::Type::kRightParen);
    match(Token::Type::kLeftBrace);
    current_ = current_->add_child(Token::Type::kStatements, forward_token());
    while (forward_token().type() != Token::Type::kRightBrace) {
        parse_statement();
    }
    match(Token::Type::kRightBrace);
    current_ = current_->parent();

    current_ = current_->parent();
}

// 解析 read 语句
// read_statement: READ LPAREN ID RPAREN SEMICOLON ;
void Parser::parse_read_statement() {
    current_ = current_->add_child(Token::Type::kReadStatement, forward_token());
    std::stringstream buffer;

    match(Token::Type::kRead);
    match(Token::Type::kLeftParen);
    if (forward_token().type() == Token::Type::kIdentity && forward_token(2).type() == Token::Type::kLeftBracket) {
        current_ = current_->add_child(Token::Type::kIdentityArray, forward_token());
        current_->add_child(forward_token());
        match(Token::Type::kIdentity);
        parse_array();
        current_ = current_->parent();
    } else if (forward_token().type() == Token::Type::kIdentity) {
        current_ = current_->add_child(Token::Type::kIdentity, forward_token());
        current_->add_child(forward_token());
        match(Token::Type::kIdentity);
        current_ = current_->parent();
    }
    match(Token::Type::kRightParen);
    match(Token::Type::kSemicolon);

    current_ = current_->parent();
}

// 解析 write 语句
// write_statement: WRITE LPAREN expression RPAREN SEMICOLON ;
void Parser::parse_write_statement() {
    current_ = current_->add_child(Token::Type::kWriteStatement, forward_token());
    std::stringstream buffer;

    match(Token::Type::kWrite);
    match(Token::Type::kLeftParen);
    parse_expression();
    match(Token::Type::kRightParen);
    match(Token::Type::kSemicolon);

    current_ = current_->parent();
}

// 解析赋值语句
// assign_statement: (ID | ID array) ASSIGN expression SEMICOLON ;
void Parser::parse_assign_statement() {
    current_ = current_->add_child(Token::Type::kAssignStatement, forward_token());
    std::stringstream buffer;

    if (forward_token().type() == Token::Type::kIdentity && forward_token(2).type() == Token::Type::kLeftBracket) {
        current_ = current_->add_child(Token::Type::kIdentityArray, forward_token());
        current_->add_child(forward_token());
        match(Token::Type::kIdentity);
        if (forward_token().type() == Token::Type::kLeftBracket) {
            parse_array();
        }
        current_ = current_->parent();
    } else if (forward_token().type() == Token::Type::kIdentity) {
        current_ = current_->add_child(Token::Type::kIdentity, forward_token());
        current_->add_child(forward_token());
        match(Token::Type::kIdentity);
        current_ = current_->parent();
    }

    match(Token::Type::kAssign);
    parse_expression();
    match(Token::Type::kSemicolon);

    current_ = current_->parent();
}

// 解析返回语句
// return_statement: RETURN (ID)? SEMICOLON ;
void Parser::parse_return_statement() {
    current_ = current_->add_child(Token::Type::kReturnStatement, forward_token());
    std::stringstream buffer;

    match(Token::Type::kReturn);
    parse_expression();
    match(Token::Type::kSemicolon);

    current_ = current_->parent();
}

// 解析定义语句
// declare_statement: declare_keyword ID(COMMA ID)* SEMICOLON ;
void Parser::parse_declare_statement() {
    current_ = current_->add_child(Token::Type::kDeclareStatement, forward_token());
    std::stringstream buffer;

    current_ = current_->add_child(Token::Type::kDeclareKeyword, forward_token());
    if (forward_token().type() == Token::Type::kInt) {
        current_->add_child(forward_token());
        match(Token::Type::kInt);
        if (forward_token().type() == Token::Type::kLeftBracket) {
            parse_array();
        }
    } else if (forward_token().type() == Token::Type::kReal) {
        current_->add_child(forward_token());
        match(Token::Type::kReal);
        if (forward_token().type() == Token::Type::kLeftBracket) {
            parse_array();
        }
    } else if (forward_token().type() == Token::Type::kVoid) {
        current_->add_child(forward_token());
        match(Token::Type::kVoid);
    } else {
        buffer << "无效的标识符 \"" << forward_token().content() << "\"";
        throw parser_exception(forward_token().position(), buffer.str());
    }
    current_ = current_->parent();

    current_->add_child(forward_token());
    match(Token::Type::kIdentity);
    while (forward_token().type() == Token::Type::kComma) {
        match(Token::Type::kComma);
        current_->add_child(forward_token());
        match(Token::Type::kIdentity);
    }
    match(Token::Type::kSemicolon);

    current_ = current_->parent();
}

// 解析定义关键字
// declare_keyword: INT | INT array | REAL | REAL array | VOID ;
void Parser::parse_declare_keyword() {
    current_ = current_->add_child(Token::Type::kDeclareKeyword, forward_token());
    std::stringstream buffer;

    if (forward_token().type() == Token::Type::kInt) {
        current_->add_child(Token::Type::kInt, forward_token());
        match(Token::Type::kInt);
        if (forward_token().type() == Token::Type::kLeftBracket) {
            parse_array();
        }
    } else if (forward_token().type() == Token::Type::kReal) {
        current_->add_child(Token::Type::kReal, forward_token());
        match(Token::Type::kReal);
        if (forward_token().type() == Token::Type::kLeftBracket) {
            parse_array();
        }
    } else if (forward_token().type() == Token::Type::kVoid) {
        current_->add_child(Token::Type::kVoid, forward_token());
        match(Token::Type::kVoid);
    } else {
        buffer << "无效的标识符 \"" << forward_token().content() << "\", 定义关键字仅允许 int / real / void";
        throw parser_exception(forward_token().position(), buffer.str());
    }

    current_ = current_->parent();
}

// 解析判别式
// condition: expression comparison_op expression ;
void Parser::parse_condition() {
    current_ = current_->add_child(Token::Type::kCondition, forward_token());

    parse_expression();
    parse_comparison_op();
    parse_expression();

    current_ = current_->parent();
}

// 解析表达式
// expression: term (add_op term)* ;
void Parser::parse_expression() {
    current_ = current_->add_child(Token::Type::kExpression, forward_token());

    parse_term();
    while (is_add_op(forward_token().type())) {
        parse_add_op();
        parse_term();
    }

    current_ = current_->parent();
}

// 解析元素
// term: factor (mul_op factor)* ;
void Parser::parse_term() {
    current_ = current_->add_child(Token::Type::kTerm, forward_token());

    parse_factor();
    while (is_mul_op(forward_token().type())) {
        parse_mul_op();
        parse_factor();
    }

    current_ = current_->parent();
}

// 解析关系运算符
// comparison_op: LT | LTE | GT | GTE | EQUAL | NEQUAL ;
void Parser::parse_comparison_op() {
    current_ = current_->add_child(Token::Type::kComparisonOp, forward_token());
    std::stringstream buffer;

    if (forward_token().type() == Token::Type::kLT) {
        current_->add_child(Token::Type::kLT, forward_token());
        match(Token::Type::kLT);
    } else if (forward_token().type() == Token::Type::kLTE) {
        current_->add_child(Token::Type::kLTE, forward_token());
        match(Token::Type::kLTE);
    } else if (forward_token().type() == Token::Type::kGT) {
        current_->add_child(Token::Type::kGT, forward_token());
        match(Token::Type::kGT);
    } else if (forward_token().type() == Token::Type::kGTE) {
        current_->add_child(Token::Type::kGTE, forward_token());
        match(Token::Type::kGTE);
    } else if (forward_token().type() == Token::Type::kEqual) {
        current_->add_child(Token::Type::kEqual, forward_token());
        match(Token::Type::kEqual);
    } else if (forward_token().type() == Token::Type::kNotEqual) {
        current_->add_child(Token::Type::kNotEqual, forward_token());
        match(Token::Type::kNotEqual);
    } else {
        buffer << "无效的关系运算符 \"" << forward_token().content() << "\", 仅允许 < <= > >= == <>";
        throw parser_exception(forward_token().position(), buffer.str());
    }

    current_ = current_->parent();
}

// 解析加减法运算符
// add_op: PLUS | MINUS ;
void Parser::parse_add_op() {
    current_ = current_->add_child(Token::Type::kAddOp, forward_token());
    std::stringstream buffer;

    if (forward_token().type() == Token::Type::kPlus) {
        current_->add_child(Token::Type::kPlus, forward_token());
        match(Token::Type::kPlus);
    } else if (forward_token().type() == Token::Type::kMinus) {
        current_->add_child(Token::Type::kMinus, forward_token());
        match(Token::Type::kMinus);
    } else {
        buffer << "无效的算数运算符 \"" << forward_token().content() << "\"";
        throw parser_exception(forward_token().position(), buffer.str());
    }

    current_ = current_->parent();
}

// 解析乘除法运算符
// mul_op: TIMES | DIVIDE ;
void Parser::parse_mul_op() {
    current_ = current_->add_child(Token::Type::kMulOp, forward_token());
    std::stringstream buffer;

    if (forward_token().type() == Token::Type::kTimes) {
        current_->add_child(Token::Type::kTimes, forward_token());
        match(Token::Type::kTimes);
    } else if (forward_token().type() == Token::Type::kDivide) {
        current_->add_child(Token::Type::kDivide, forward_token());
        match(Token::Type::kDivide);
    } else {
        buffer << "无效的算数运算符 \"" << forward_token().content() << "\"";
        throw parser_exception(forward_token().position(), buffer.str());
    }

    current_ = current_->parent();
}

// 解析元素
// factor: REAL_LITERAL | INTEGER_LITERAL | ID | LPAREN expression RPAREN | ID array | function_call ;
void Parser::parse_factor() {
    current_ = current_->add_child(Token::Type::kFactor, forward_token());
    std::stringstream buffer;

    if (forward_token().type() == Token::Type::kRealLiteral) {
        current_->add_child(forward_token());
        match(Token::Type::kRealLiteral);
    } else if (forward_token().type() == Token::Type::kIntegerLiteral) {
        current_->add_child(forward_token());
        match(Token::Type::kIntegerLiteral);
    } else if (forward_token(1).type() == Token::Type::kIdentity) {
        if (forward_token(2).type() == Token::Type::kLeftParen) {
            parse_function_call();
        } else {
            if (forward_token(2).type() == Token::Type::kLeftBracket) {
                current_ = current_->add_child(Token::Type::kIdentityArray, forward_token());
                current_->add_child(forward_token());
                match(Token::Type::kIdentity);
                parse_array();
                current_ = current_->parent();
            } else {
                current_->add_child(forward_token());
                match(Token::Type::kIdentity);
            }

        }
    } else if (forward_token().type() == Token::Type::kLeftParen) {
        match(Token::Type::kLeftParen);
        parse_expression();
        match(Token::Type::kRightParen);
    } else {
        buffer << "无效的标识符 \"" << forward_token().content() << "\"";
        throw parser_exception(forward_token().position(), buffer.str());
    }

    current_ = current_->parent();
}

// 解析数组
// array: LBRACKET INTEGER_LITERAL RBRACKET ;
void Parser::parse_array() {
    current_ = current_->add_child(Token::Type::kArray, forward_token());
    std::stringstream buffer;

    match(Token::Type::kLeftBracket);
    if (forward_token().type() == Token::Type::kIntegerLiteral) {
        current_->add_child(forward_token());
        match(Token::Type::kIntegerLiteral);
    } else if (forward_token().type() == Token::Type::kIdentity) {
        current_->add_child(forward_token());
        match(Token::Type::kIdentity);
    } else {
        buffer << "无效的标识符 \"" << forward_token().content() << "\", 数组括号中仅允许整数常量或变量(变量仅限用作左值)";
        throw parser_exception(forward_token().position(), buffer.str());
    }
    match(Token::Type::kRightBracket);

    current_ = current_->parent();
}

// 解析函数调用
// function_call: ID LPAREN ((ID | REAL_LITERAL | INTEGER_LITERAL) (COMMA (ID | REAL_LITERAL | INTEGER_LITERAL)*)?) RPAREN ;
void Parser::parse_function_call() {
    current_ = current_->add_child(Token::Type::kFunctionCall, forward_token());
    std::stringstream buffer;

    current_->add_child(forward_token());
    match(Token::Type::kIdentity);
    current_ = current_->add_child(Token::Type::kFunctionCallParameters, forward_token());
    match(Token::Type::kLeftParen);
    if (is_function_call_valid_parameter(forward_token().type())) {
        do {
            if (forward_token().type() == Token::Type::kComma) {
                match(Token::Type::kComma);
            }
            parse_expression();
        } while (forward_token().type() == Token::Type::kComma);
    }
    match(Token::Type::kRightParen);
    current_ = current_->parent();

    current_ = current_->parent();
}

void Parser::print_ast() const {
    root_->print();
}

bool Parser::is_declare_keyword(const Token::Type &type) {
    return type == Token::Type::kInt || type == Token::Type::kReal || type == Token::Type::kVoid;
}

bool Parser::is_comparison_op(const Token::Type &type) {
    return type == Token::Type::kLT || type == Token::Type::kLTE || type == Token::Type::kGT ||
           type == Token::Type::kGTE || type == Token::Type::kEqual || type == Token::Type::kNotEqual;
}

bool Parser::is_add_op(const Token::Type &type) {
    return type == Token::Type::kPlus || type == Token::Type::kMinus;
}

bool Parser::is_mul_op(const Token::Type &type) {
    return type == Token::Type::kTimes || type == Token::Type::kDivide;
}

bool Parser::is_function_call_valid_parameter(const Token::Type &type) {
    return type == Token::Type::kIntegerLiteral || type == Token::Type::kRealLiteral || type == Token::Type::kIdentity;
}

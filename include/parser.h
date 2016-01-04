#ifndef CMM_PARSER_H
#define CMM_PARSER_H

#include <vector>
#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "exceptions.h"

class Parser {
public:
    Parser(const Lexer &lexer, const int &total) {
        lexer_ = lexer;
        lookahead_.resize(5);
        total_ = total;
        index_ = 0;
        for (int i = 0; i < total_; i++) {
            consume();
        }

        root = new AbstractSyntaxNode(get_forward(1));
        current = root;
    }

    void consume() {
        lookahead_[index_] = lexer_.next_token();
        index_ = (index_ + 1) % total_;
    }

    Token get_forward(const int &step) const {
        return lookahead_[(index_ + step - 1) % total_];
    }

    Token match(const Token::LexerType &x) {
        Token forward_token = get_forward(1);
        if (forward_token.type() == x) {
            consume();
            return forward_token;
        } else {
            std::stringstream buffer;
            buffer << "错误 Token 类型 \"" << forward_token.type_name() <<
                      "\", 期望 Token 类型 \"" << Token::token_type_name(x) << "\"";
            throw parser_exception(buffer.str());
        }
    }

private:
    Lexer lexer_;
    std::vector<Token> lookahead_;
    int total_;
    int index_;
    AbstractSyntaxNode *root;
    AbstractSyntaxNode *current;
};

#endif //CMM_PARSER_H

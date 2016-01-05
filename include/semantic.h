#ifndef CMM_SEMANTIC_H
#define CMM_SEMANTIC_H

#include <iostream>
#include "ast.h"
#include "symbol.h"
#include "scope.h"
#include "ir.h"

class Semantic {
public:
    Semantic(AbstractSyntaxNode *root) {
        root_ = root;
        current_ = root_;
    }

    void analyse() {
        switch (current_token_type()) {
            case Token::Type::kProgram:
                analyse_program();
                break;
            case Token::Type::kFunction:
                analyse_function();
                break;
            default:
                break;
        }
    }

    Token analyse_program() {
        for (int i = 0; i < children_size(); ++i) {
            down_walk_up(child(i));
        }
        return Token(Token::Type::kProgram, current_->token().position());
    }

    Token analyse_function() {
        int offset = 0;
        Token declare_keyword = down_walk_up(child(offset++));
        Token identity = down_walk_up(child(offset++));

        std::vector<std::pair<Token, Token> > parameter;
        while (offset < children_size() && child_type(offset) != Token::Type::kStatement) {
            Token first = down_walk_up(child(offset++));
            Token second = down_walk_up(child(offset++));
            parameter.push_back(std::pair<Token, Token>(first, second));
        }

        Token return_statement;
        if (offset < children_size()) {
            while (offset < children_size()) {
                Token statement = down_walk_up(child(offset++));
                if (statement.type() == Token::Type::kReturnStatement) {

                }
            }
        } else {

        }
        return Token(Token::Type::kFunction, current_->token().position());
    }

private:
    AbstractSyntaxNode *root_;
    AbstractSyntaxNode *current_;
    ScopeTree tree_;
    std::vector<IR> ir_;

    Token down_walk_up(AbstractSyntaxNode *p) {
        current_ = p;
        Token result;
        switch (current_token_type()) {
            case Token::Type::kFunction:
                result = analyse_function();
                break;
            case Token::Type::kStatement:
                break;
            default:
                break;
        }
        current_ = current_->parent();
        return result;
    }

    AbstractSyntaxNode *child(int offset) {
        return current_->children()[offset];
    }

    Token::Type child_type(int offset) {
        return child(offset)->token().type();
    }

    int children_size() {
        return (int)current_->children().size();
    }

    Token::Type current_token_type() {
        return current_->token().type();
    }
};

#endif //CMM_SEMANTIC_H

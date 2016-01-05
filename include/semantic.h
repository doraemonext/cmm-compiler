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
        analyse_program();
    }

    Token analyse_program() {
        for (int i = 0; i < children_size(); ++i) {
            if (current_token_type() == Token::Type::kFunction) {
                analyse_function(i);
            } else if (current_token_type() == Token::Type::kStatement) {
                analyse_statement(i);
            }
        }
        return Token(Token::Type::kProgram, current_->token().position());
    }

    Token analyse_function(const int &pos) {
        current_ = child(pos);

//        int offset = 0;
//        Token declare_keyword = analyse_declare_keyword(child(offset++));
//        Token identity = analyse_declare_identity(child(offset++));
//
//        std::vector<std::pair<Token, Token> > parameter;
//        while (offset < children_size() && child_type(offset) != Token::Type::kStatement) {
//            Token first = down_walk_up(child(offset++));
//            Token second = down_walk_up(child(offset++));
//            parameter.push_back(std::pair<Token, Token>(first, second));
//        }
//
//        Token return_statement;
//        if (offset < children_size()) {
//            while (offset < children_size()) {
//                Token statement = down_walk_up(child(offset++));
//                if (statement.type() == Token::Type::kReturnStatement) {
//                    return_statement = statement;
//                }
//            }
//        } else {
//            return_statement = Token(Token::Type::kVoid, current_->token().position());
//        }
        return Token(Token::Type::kFunction, current_->token().position());

        current_ = current_->parent();
    }

    Token analyse_statement(const int &pos) {
        return Token(Token::Type::kStatement, current_->token().position());
    }

private:
    AbstractSyntaxNode *root_;
    AbstractSyntaxNode *current_;
    ScopeTree tree_;
    std::vector<PCode> ir_;

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

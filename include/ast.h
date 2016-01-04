#ifndef CMM_AST_H
#define CMM_AST_H

#include <vector>
#include "token.h"

class AbstractSyntaxNode {
public:
    AbstractSyntaxNode() : token_(Token()), parent_(nullptr) { }

    AbstractSyntaxNode(const Token &token, AbstractSyntaxNode *parent = nullptr) : token_(token), parent_(parent) { }

    AbstractSyntaxNode *add_child(AbstractSyntaxNode *child) {
        children_.push_back(child);
        return children_.back();
    }

    const Token &token() const {
        return token_;
    }

    void set_token(const Token &token) {
        token_ = token;
    }

    const std::vector<AbstractSyntaxNode *> &children() const {
        return children_;
    }

private:
    Token token_;
    AbstractSyntaxNode *parent_;
    std::vector<AbstractSyntaxNode *> children_;
};

#endif //CMM_AST_H

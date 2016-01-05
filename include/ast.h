#ifndef CMM_AST_H
#define CMM_AST_H

#include <iostream>
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

    AbstractSyntaxNode *add_child(const Token::Type &type, const Token &forward_token) {
        AbstractSyntaxNode *child = new AbstractSyntaxNode(Token(type, forward_token.position()), this);
        children_.push_back(child);
        return children_.back();
    }

    AbstractSyntaxNode *add_child(const Token &token) {
        AbstractSyntaxNode *child = new AbstractSyntaxNode(token, this);
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

    AbstractSyntaxNode *child(const int &pos) {
        return children_[pos];
    }

    AbstractSyntaxNode *parent() const {
        return parent_;
    }

    void print(const int &indent = 0) const {
        for (int i = 0; i < indent; ++i) {
            std::cout << " ";
        }
        std::cout << "Token: \"" << token_.content() << "\"<\"" << token_.type_name() << "\"> (Children: " << children_.size() << ")" << std::endl;
        for (std::vector<AbstractSyntaxNode *>::const_iterator it = children_.begin(); it != children_.end(); ++it) {
            (*it)->print(indent + 4);
        }
    }

private:
    Token token_;
    AbstractSyntaxNode *parent_;
    std::vector<AbstractSyntaxNode *> children_;
};

#endif //CMM_AST_H

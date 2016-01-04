#ifndef CMM_SCOPE_H
#define CMM_SCOPE_H

#include <iostream>
#include <map>
#include <string>
#include <list>
#include "exceptions.h"
#include "symbol.h"

class ScopeNode {
public:
    ScopeNode();

    ScopeNode(ScopeNode *enclosing_scope);

    Symbol &resolve(const std::string &name) const;

    void define(const Symbol &symbol);

    ScopeNode *get_enclosing_scope() const;

    ScopeNode *push(ScopeNode *child);

    int level() const;

    void print() const;

private:
    std::map<std::string, Symbol> symbols_;
    int level_;
    ScopeNode *enclosing_scope_;
    std::list<ScopeNode *> children_;
};

class ScopeTree {
public:
    ScopeTree();

    Symbol &resolve(const std::string &name) const;

    void define(const Symbol &symbol) const;

    void push();

    void pop();

    void print() const;

private:
    ScopeNode *root_;
    ScopeNode *current_;
};

#endif //CMM_SCOPE_H

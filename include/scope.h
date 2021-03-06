#ifndef CMM_SCOPE_H
#define CMM_SCOPE_H

#include <iostream>
#include <map>
#include <string>
#include <list>
#include "exceptions.h"
#include "symbol.h"
#include "token.h"

class ScopeNode {
public:
    ScopeNode();

    ScopeNode(ScopeNode *enclosing_scope);

    Symbol &resolve(const std::string &name);

    ScopeNode *resolve_scope(const std::string &name);

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

    Symbol &resolve(const std::string &name);

    ScopeNode *resolve_scope(const std::string &name);

    void define(const Symbol &symbol);

    void push();

    void pop();

    ScopeNode *current();

    void print() const;

private:
    ScopeNode *root_;
    ScopeNode *current_;
};

#endif //CMM_SCOPE_H

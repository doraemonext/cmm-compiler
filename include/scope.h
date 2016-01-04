#ifndef CMM_SCOPE_H
#define CMM_SCOPE_H

#include <iostream>
#include <map>
#include <string>
#include <list>
#include "symbol.h"

class ScopeNode {
public:
    ScopeNode(ScopeNode *enclosing_scope = nullptr) : enclosing_scope_(enclosing_scope) {
        if (enclosing_scope == nullptr) {
            level_ = 0;
        } else {
            level_ = enclosing_scope->level() + 1;
        }
    }

    const Symbol &resolve(const std::string &name) const {
        std::map<std::string, Symbol>::const_iterator it = symbols_.find(name);
        if (it != symbols_.end()) {
            return it->second;
        } else {
            ScopeNode *enclosing_scope = get_enclosing_scope();
            if (enclosing_scope) {
                return enclosing_scope->resolve(name);
            } else {
                throw scope_not_found();
            }
        }
    }

    void define(const Symbol &symbol) {
        symbols_.insert(std::pair<std::string, Symbol>(symbol.name(), symbol));
    }

    ScopeNode *get_enclosing_scope() const {
        return enclosing_scope_;
    }

    ScopeNode *push(ScopeNode *child) {
        children_.push_back(child);
        return children_.back();
    }

    int level() const {
        return level_;
    }

    void print() const {
        std::cout << "Level: " << level() << std::endl;
        for (std::map<std::string, Symbol>::const_iterator it = symbols_.begin(); it != symbols_.end(); ++it) {
            std::cout << "Name: " << it->first << " \nSymbol:\n";
            it->second.print();
            std::cout << std::endl;
        }

        for (std::list<ScopeNode *>::const_iterator it = children_.begin(); it != children_.end(); ++it) {
            (*it)->print();
        }
    }

private:
    std::map<std::string, Symbol> symbols_;
    int level_;
    ScopeNode *enclosing_scope_;
    std::list<ScopeNode *> children_;
};

#endif //CMM_SCOPE_H

#include "include/scope.h"

ScopeNode::ScopeNode() : enclosing_scope_(nullptr) {
    ScopeNode(nullptr);
}

ScopeNode::ScopeNode(ScopeNode *enclosing_scope) : enclosing_scope_(enclosing_scope) {
    if (enclosing_scope == nullptr) {
        level_ = 0;
    } else {
        level_ = enclosing_scope->level() + 1;
    }
}

const Symbol &ScopeNode::resolve(const std::string &name) const {
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

inline void ScopeNode::define(const Symbol &symbol) {
    symbols_.insert(std::pair<std::string, Symbol>(symbol.name(), symbol));
}

inline ScopeNode *ScopeNode::get_enclosing_scope() const {
    return enclosing_scope_;
}

inline ScopeNode *ScopeNode::push(ScopeNode *child) {
    children_.push_back(child);
    return children_.back();
}

inline int ScopeNode::level() const {
    return level_;
}

void ScopeNode::print() const {
    std::cout << "--------------------------" << std::endl;
    std::cout << "Level: " << level() << std::endl;
    std::cout << "--------------------------" << std::endl;
    for (std::map<std::string, Symbol>::const_iterator it = symbols_.begin(); it != symbols_.end(); ++it) {
        it->second.print();
        std::cout << std::endl;
    }

    for (std::list<ScopeNode *>::const_iterator it = children_.begin(); it != children_.end(); ++it) {
        (*it)->print();
    }
}

ScopeTree::ScopeTree() {
    root_ = new ScopeNode();
    current_ = root_;
}

inline const Symbol &ScopeTree::resolve(const std::string &name) const {
    return current_->resolve(name);
}

inline void ScopeTree::define(const Symbol &symbol) const {
    current_->define(symbol);
}

inline void ScopeTree::push() {
    ScopeNode *node = new ScopeNode(current_);
    current_ = current_->push(node);
}

inline void ScopeTree::pop() {
    current_ = current_->get_enclosing_scope();
}

inline void ScopeTree::print() const {
    root_->print();
}

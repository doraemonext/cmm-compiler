#include <iostream>
#include <string>
#include <vector>
#include "include/token.h"
#include "include/lexer.h"
#include "include/utils.h"
#include "include/symbol.h"
#include "include/scope.h"

using namespace std;

int main() {
    Lexer lexer("int a;\n   int b, c = 10;\n while (c >= 15 + 2) /* aaa_ { b = b + 1; };\nfunc int abc(int a, int b) { read(c); }");
    try {
        Token token = lexer.next_token();
        while (token.type() != Token::Type::kEOF) {
            // std::cout << token << std::endl;
            token = lexer.next_token();
        }
    } catch (const lexer_exception &e) {
        // std::cout << e.what() << std::endl;
    }

    ScopeNode *root = new ScopeNode();
    ScopeNode *current = root;

    Symbol s1("i", 9);
    current->define(s1);

    Symbol s2("f", Symbol::Type::kReal, {Symbol("x", 0), Symbol("y", 0.0)});
    current->define(s2);

    ScopeNode *method_f = new ScopeNode(current);
    current = current->push(method_f);

    Symbol s3("x", 0);
    Symbol s4("y", 0.0);
    current->define(s3);
    current->define(s4);

    root->print();

    return 0;
}
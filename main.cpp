#include <iostream>
#include <string>
#include <vector>
#include "include/token.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/utils.h"
#include "include/symbol.h"
#include "include/scope.h"

using namespace std;

int main() {
    Lexer lexer("int a;\n   int b, c = 10;\n while (c >= 15 + 2) /* aaa_ { b = b + 1; };\nfunc int abc(int a, int b) { read(c); }");
    Parser parser(lexer, 2);
//    try {
//        Token token = lexer.next_token();
//        while (token.type() != Token::Type::kEOF) {
//            std::cout << token << std::endl;
//            token = lexer.next_token();
//        }
//    } catch (const lexer_exception &e) {
//        std::cout << e.what() << std::endl;
//    }

//    ScopeTree *tree = new ScopeTree();
//
//    Symbol s1("i", 9);
//    tree->define(s1);
//
//    Symbol s2("f", Symbol::Type::kReal, {Symbol("x", 0), Symbol("y", 0.0)});
//    tree->define(s2);
//
//    tree->push();
//
//    Symbol s3("x", 0);
//    Symbol s4("y", 0.0);
//    tree->define(s3);
//    tree->define(s4);

    return 0;
}
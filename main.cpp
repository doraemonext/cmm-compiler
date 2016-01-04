#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include "include/token.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/utils.h"
#include "include/symbol.h"
#include "include/scope.h"

using namespace std;

int main() {
    Lexer lexer("int a, b, c; real[5] d;;;;");
    Parser parser(lexer, 2);
    parser.parse_program();
    parser.print_ast();
//    try {
//        Token token = lexer.next_token();
//        while (token.type() != Token::Type::kEOF) {
//            token.print();
//            cout << endl;
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
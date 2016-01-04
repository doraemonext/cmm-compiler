#include <iostream>
#include <string>
#include "include/token.h"
#include "include/lexer.h"
#include "include/utils.h"
#include "include/symbol.h"

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

    Symbol *symbol = new Symbol("value_a", Symbol::Type::kInt);
    symbol->print();

    return 0;
}
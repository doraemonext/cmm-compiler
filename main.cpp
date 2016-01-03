#include <iostream>
#include <string>
#include "include/token.h"
#include "include/lexer.h"
#include "include/utils.h"

using namespace std;

int main() {
    Lexer lexer("int a;\n   int b, c = 10;\n while (c >= 15 + 2) { b = b + 1; };\n");
    Token token = lexer.next_token();
    while (token.type() != Token::Type::kEOF) {
        std::cout << token << std::endl;
        token = lexer.next_token();
    }

    return 0;
}
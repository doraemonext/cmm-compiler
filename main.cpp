#include <iostream>
#include <string>
#include "include/token.h"
#include "include/lexer.h"
#include "include/utils.h"

using namespace std;

int main() {
    Lexer lexer("aint\nbbt");
    lexer.match('a');
    lexer.match('i');
    lexer.match('n');
    lexer.match('t');
    lexer.match('b');
    lexer.match('c');
    return 0;
}
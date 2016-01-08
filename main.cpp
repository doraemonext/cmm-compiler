#include <iostream>
#include <vector>
#include <fstream>
#include "include/token.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/semantic.h"

using namespace std;

int main() {
    std::ifstream t("input.cmm");
    std::stringstream buffer;
    buffer << t.rdbuf();

    Lexer lexer(buffer.str());
    Parser parser(lexer, 2);
    parser.parse_program();
    cout << endl << "语法树:" << endl << endl;
    parser.print_ast();
    cout << endl;

    Semantic semantic(parser.ast());
    try {
        semantic.analyse();
        cout << endl << "中间代码:" << endl << endl;
        semantic.print_ir();
    } catch (const scope_critical_error &e) {
        semantic.print_error_messages();
    }
    semantic.print_warning_messages();

    return 0;
}
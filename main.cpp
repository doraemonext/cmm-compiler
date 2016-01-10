#include <iostream>
#include <vector>
#include <fstream>
#include "include/token.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/semantic.h"
#include "include/simulator.h"

using namespace std;

int main() {
    std::ifstream t("input.cmm");
    std::stringstream buffer;
    buffer << t.rdbuf();

    Lexer lexer(buffer.str());
    Parser parser(lexer, 2);
    try {
        parser.parse_program();
    } catch (const parser_exception &e) {
        std::cout << e.what() << std::endl;
        exit(0);
    } catch (const lexer_exception &e) {
        std::cout << e.what() << std::endl;
        exit(0);
    }
    cout << endl << "语法树:" << endl << endl;
    parser.print_ast();
    cout << endl;

    Semantic semantic(parser.ast());
    try {
        semantic.analyse();
        cout << endl << "中间代码:" << endl << endl;
        semantic.print_ir();
        semantic.print_warning_messages();
    } catch (const scope_critical_error &e) {
        semantic.print_error_messages();
        semantic.print_warning_messages();
        return 0;
    }

    Simulator simulator(semantic.ir());
    try {
        cout << endl << "运行结果:" << endl << endl;
        simulator.run();
    } catch (const simulator_error &e) {
        std::cout << "[中间代码错误] " << e.what() << std::endl;
    }

    return 0;
}
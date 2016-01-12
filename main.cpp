#include <iostream>
#include <vector>
#include <fstream>
#include "include/token.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/semantic.h"
#include "include/simulator.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Error: 需要传入源码所在路径作为第一个参数" << std::endl;
        exit(1);
    }
    std::ifstream t(argv[1]);
    std::stringstream buffer;
    buffer << t.rdbuf();

    try {
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
    } catch (const parser_exception &e) {
        std::cout << e.what() << std::endl;
        exit(0);
    } catch (const lexer_exception &e) {
        std::cout << e.what() << std::endl;
        exit(0);
    }

    return 0;
}
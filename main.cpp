#include <iostream>
#include <vector>
#include <fstream>
#include "include/token.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/semantic.h"
#include "include/ir.h"

using namespace std;

int main() {
    std::ifstream t("input.cmm");
    std::stringstream buffer;
    buffer << t.rdbuf();

    Lexer lexer(buffer.str());
    Parser parser(lexer, 2);
    parser.parse_program();
    parser.print_ast();
    cout << endl;

    Semantic semantic(parser.ast());
    try {
        semantic.analyse();
        semantic.print_ir();
    } catch (const scope_critical_error &e) {
        semantic.print_error_messages();
        semantic.print_warning_messages();
    }

//    IR ir;
//    ir.add(PCode(PCode::Type::kStartFunc, "factor", 0));
//    ir.add(PCode(PCode::Type::kReturnIntegerArray, "a"));
//    std::cout << ir;

    // std::vector<IR> ir;
    // ir.push_back(IR(OpCode::kAdd, Var(Var::Type::kInt, 10), Var(Var::Type::kReal, 10.0), Var(Var::Type::kInt, 10)));
    // cout << ir[0] << endl;

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
#ifndef CMM_SIMULATOR_H
#define CMM_SIMULATOR_H

#include <vector>
#include <string>
#include <map>
#include <deque>
#include "ir.h"
#include "scope.h"
#include "exceptions.h"

class Simulator {
public:
    Simulator(const IR &ir) : ir_(ir), eip_(0) { }

    void var_integer(const PCode &code);

    void var_integer_array(const PCode &code);

    void var_real(const PCode &code);

    void var_real_array(const PCode &code);

    void push_integer(const PCode &code);

    void push_integer_array(const PCode &code);

    void push_real(const PCode &code);

    void push_real_array(const PCode &code);

    // 运行中间代码
    void run() {
        int pos = 0;

        // 添加主函数调用
        ir_.add(PCode(PCode::Type::kCall, "main"));
        ir_.add(PCode(PCode::Type::kExit, "~"));

        // 记录所有 Label 和函数 Label 相对位置
        for (pos = 0; pos < ir_.size(); ++pos) {
            const PCode &line = ir_.at(pos);

            if (line.type() == PCode::Type::kLabel) {
                label_table_[line.first()] = pos;
            } else if (line.type() == PCode::Type::kStartFunc) {
                func_table_[line.first()] = std::pair<int, int>(pos, -1);
            } else if (line.type() == PCode::Type::kEndFunc) {
                func_table_[line.first()] = std::pair<int, int>(func_table_[line.first()].first, pos);
            }
        }

        // 开始执行
        for (pos = 0; pos < ir_.size(); ++pos) {
            const PCode &line = ir_.at(pos);

            switch (line.type()) {
                case PCode::Type::kStartFunc:
                    break;
                case PCode::Type::kArgInteger:
                    break;
                case PCode::Type::kArgIntegerArray:
                    break;
                case PCode::Type::kArgReal:
                    break;
                case PCode::Type::kArgRealArray:
                    break;
                case PCode::Type::kReturn:
                    break;
                case PCode::Type::kEndFunc:
                    break;
                case PCode::Type::kCall:
                    break;
                case PCode::Type::kVarInteger:
                    var_integer(line);
                    break;
                case PCode::Type::kVarIntegerArray:
                    var_integer_array(line);
                    break;
                case PCode::Type::kVarReal:
                    var_real(line);
                    break;
                case PCode::Type::kVarRealArray:
                    var_real_array(line);
                    break;
                case PCode::Type::kPushInteger:
                    push_integer(line);
                    break;
                case PCode::Type::kPushIntegerArray:
                    push_integer_array(line);
                    break;
                case PCode::Type::kPushReal:
                    push_real(line);
                    break;
                case PCode::Type::kPushRealArray:
                    push_real_array(line);
                    break;
                case PCode::Type::kPop:
                    break;
                case PCode::Type::kPopInteger:
                    break;
                case PCode::Type::kPopIntegerArray:
                    break;
                case PCode::Type::kPopReal:
                    break;
                case PCode::Type::kPopRealArray:
                    break;
                case PCode::Type::kAdd:
                    break;
                case PCode::Type::kSub:
                    break;
                case PCode::Type::kMul:
                    break;
                case PCode::Type::kDiv:
                    break;
                case PCode::Type::kMod:
                    break;
                case PCode::Type::kCompareEqual:
                    break;
                case PCode::Type::kCompareNotEqual:
                    break;
                case PCode::Type::kCompareGreaterThan:
                    break;
                case PCode::Type::kCompareLessThan:
                    break;
                case PCode::Type::kCompareGreaterEqual:
                    break;
                case PCode::Type::kCompareLessEqual:
                    break;
                case PCode::Type::kAnd:
                    break;
                case PCode::Type::kOr:
                    break;
                case PCode::Type::kNot:
                    break;
                case PCode::Type::kNegative:
                    break;
                case PCode::Type::kJump:
                    break;
                case PCode::Type::kJumpZero:
                    break;
                case PCode::Type::kJumpNotZero:
                    break;
                case PCode::Type::kPrint:
                    break;
                case PCode::Type::kReadInt:
                    break;
                case PCode::Type::kReadIntArray:
                    break;
                case PCode::Type::kReadReal:
                    break;
                case PCode::Type::kReadRealArray:
                    break;
                case PCode::Type::kExit:
                    break;
                default:
                    break;
            }
        }
    }

private:
    IR ir_;
    std::deque<Symbol> stack_;
    ScopeTree tree_;
    std::map<std::string, std::pair<int, int> > func_table_;
    std::map<std::string, int> label_table_;
    int eip_;
};

#endif //CMM_SIMULATOR_H

void Simulator::var_integer(const PCode &code) {
    tree_.define(Symbol(code.first(), 0, false));
}

void Simulator::var_integer_array(const PCode &code) {
    tree_.define(Symbol(code.first(), std::vector<int>(std::stoul(code.second()), 0), true));
}

void Simulator::var_real(const PCode &code) {
    tree_.define(Symbol(code.first(), 0.0, false));
}

void Simulator::var_real_array(const PCode &code) {
    tree_.define(Symbol(code.first(), std::vector<double>(std::stoul(code.second()), 0.0), true));
}

void Simulator::push_integer(const PCode &code) {

}

void Simulator::push_integer_array(const PCode &code) {

}

void Simulator::push_real(const PCode &code) {

}

void Simulator::push_real_array(const PCode &code) {

}

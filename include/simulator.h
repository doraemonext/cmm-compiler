#ifndef CMM_SIMULATOR_H
#define CMM_SIMULATOR_H

#include <vector>
#include <string>
#include <map>
#include <deque>
#include <cmath>
#include "ir.h"
#include "scope.h"
#include "exceptions.h"
#include "utils.h"

class Simulator {
public:
    Simulator(const IR &ir) : ir_(ir), eip_(0), inloop_(false) { }

    void var_integer(const PCode &code);

    void var_integer_array(const PCode &code);

    void var_real(const PCode &code);

    void var_real_array(const PCode &code);

    void push_integer(const PCode &code);

    void push_integer_array(const PCode &code);

    void push_real(const PCode &code);

    void push_real_array(const PCode &code);

    void pop(const PCode &code);

    void pop_identity(const PCode &code);

    void pop_array(const PCode &code);

    void add(const PCode &code);

    void sub(const PCode &code);

    void mul(const PCode &code);

    void divide(const PCode &code);

    void mod(const PCode &code);

    void compare_equal(const PCode &code);

    void compare_not_equal(const PCode &code);

    void compare_greater_than(const PCode &code);

    void compare_less_than(const PCode &code);

    void compare_greater_equal(const PCode &code);

    void compare_less_equal(const PCode &code);

    void jump(const PCode &code);

    void jump_zero(const PCode &code);

    void jump_not_zero(const PCode &code);

    void label(const PCode &code);

    void print(const PCode &code);

    void read_int(const PCode &code);

    void read_int_array(const PCode &code);

    void read_real(const PCode &code);

    void read_real_array(const PCode &code);

    void exit_program(const PCode &code);

    // 每次运行一条指令
    int run_instruction() {
        if (eip_ >= ir_.size()) {
            return 0;
        }
//        std::cout << "Eip: " << eip_ << std::endl;

        const PCode &line = ir_.at(eip_);
        switch (line.type()) {
            case PCode::Type::kLabel:
                label(line);
                break;
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
                pop(line);
                break;
            case PCode::Type::kPopInteger:
            case PCode::Type::kPopReal:
                pop_identity(line);
                break;
            case PCode::Type::kPopIntegerArray:
            case PCode::Type::kPopRealArray:
                pop_array(line);
                break;
            case PCode::Type::kAdd:
                add(line);
                break;
            case PCode::Type::kSub:
                sub(line);
                break;
            case PCode::Type::kMul:
                mul(line);
                break;
            case PCode::Type::kDiv:
                divide(line);
                break;
            case PCode::Type::kMod:
                mod(line);
                break;
            case PCode::Type::kCompareEqual:
                compare_equal(line);
                break;
            case PCode::Type::kCompareNotEqual:
                compare_not_equal(line);
                break;
            case PCode::Type::kCompareGreaterThan:
                compare_greater_than(line);
                break;
            case PCode::Type::kCompareLessThan:
                compare_less_than(line);
                break;
            case PCode::Type::kCompareGreaterEqual:
                compare_greater_equal(line);
                break;
            case PCode::Type::kCompareLessEqual:
                compare_less_equal(line);
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
                jump(line);
                break;
            case PCode::Type::kJumpZero:
                jump_zero(line);
                break;
            case PCode::Type::kJumpNotZero:
                jump_not_zero(line);
                break;
            case PCode::Type::kPrint:
                print(line);
                break;
            case PCode::Type::kReadInt:
                read_int(line);
                break;
            case PCode::Type::kReadIntArray:
                read_int_array(line);
                break;
            case PCode::Type::kReadReal:
                read_real(line);
                break;
            case PCode::Type::kReadRealArray:
                read_real_array(line);
                break;
            case PCode::Type::kExit:
                exit_program(line);
                break;
            default:
                break;
        }
        return -1;
    }

    // 运行中间代码
    void run() {
        int pos = 0;

        // 添加主函数调用
        // ir_.add(PCode(PCode::Type::kCall, "main"));
        // ir_.add(PCode(PCode::Type::kExit, "~"));

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

        eip_ = 0;
        int return_status = -1;
        int tmp = 3000;
        while (tmp > 0) {
            return_status = run_instruction();
            if (return_status >= 0) {
                break;
            }
            tmp--;
        }
        if (tmp <= 0) {
            std::cout << "over 3000 times." << std::endl;
        }
    }

    int eip() const {
        return eip_;
    }

    void inc_eip() {
        eip_++;
    }

    void set_eip(const int &pos) {
        eip_ = pos;
    }

private:
    IR ir_;
    std::deque<StackSymbol> stack_;
    ScopeTree tree_;
    std::map<std::string, std::pair<int, int> > func_table_;
    std::map<std::string, int> label_table_;
    int eip_;
    int inloop_;

    int get_second_parameter(const std::string &str) {
        if (Recognition::is_integer(str)) {
            return std::stoi(str);
        } else {
            Symbol symbol = tree_.resolve(str);
            return symbol.int_value();
        }
    }
};

#endif //CMM_SIMULATOR_H

void Simulator::var_integer(const PCode &code) {
    tree_.define(Symbol(code.first(), 0, false));
    inc_eip();
}

void Simulator::var_integer_array(const PCode &code) {
    tree_.define(Symbol(code.first(), std::vector<int>(std::stoul(code.second()), 0), true));
    inc_eip();
}

void Simulator::var_real(const PCode &code) {
    tree_.define(Symbol(code.first(), 0.0, false));
    inc_eip();
}

void Simulator::var_real_array(const PCode &code) {
    tree_.define(Symbol(code.first(), std::vector<double>(std::stoul(code.second()), 0.0), true));
    inc_eip();
}

void Simulator::push_integer(const PCode &code) {
    if (Recognition::is_integer(code.first())) {
        stack_.push_back(StackSymbol(std::stoi(code.first())));
    } else {
        Symbol symbol = tree_.resolve(code.first());
        if (symbol.is_assigned()) {
            stack_.push_back(StackSymbol(symbol.int_value()));
        } else {
            throw simulator_error(eip(), "变量 \"" + code.first() + "\" 未初始化而直接使用");
        }
    }
    inc_eip();
}

void Simulator::push_integer_array(const PCode &code) {
    Symbol symbol = tree_.resolve(code.first());
    if (Recognition::is_integer(code.second())) {
        stack_.push_back(StackSymbol(symbol.int_array().at(std::stoul(code.second()))));
    } else {
        Symbol offset = tree_.resolve(code.second());
        stack_.push_back(StackSymbol(symbol.int_array().at((unsigned long)offset.int_value())));
    }
    inc_eip();
}

void Simulator::push_real(const PCode &code) {
    if (Recognition::is_real(code.first())) {
        stack_.push_back(StackSymbol(std::stod(code.first())));
    } else {
        Symbol symbol = tree_.resolve(code.first());
        stack_.push_back(StackSymbol(symbol.real_value()));
    }
    inc_eip();
}

void Simulator::push_real_array(const PCode &code) {
    Symbol symbol = tree_.resolve(code.first());
    if (Recognition::is_integer(code.second())) {
        stack_.push_back(StackSymbol(symbol.real_array().at(std::stoul(code.second()))));
    } else {
        Symbol offset = tree_.resolve(code.second());
        stack_.push_back(StackSymbol(symbol.real_array().at((unsigned long)offset.int_value())));
    }
    inc_eip();
}

void Simulator::pop(const PCode &code) {
    stack_.pop_back();
    inc_eip();
}

void Simulator::pop_identity(const PCode &code) {
    StackSymbol back = stack_.back();
    stack_.pop_back();
    Symbol &symbol = tree_.resolve(code.first());

    if (symbol.type() == Symbol::Type::kInt) {
        if (back.type() == StackSymbol::Type::kInt) {
            symbol.set_value((int) back.int_value());
        } else if (back.type() == StackSymbol::Type::kReal) {
            symbol.set_value((int) back.real_value());
        } else {
            throw simulator_error(eip(), "无法取出栈顶元素");
        }
    } else if (symbol.type() == Symbol::Type::kReal) {
        if (back.type() == StackSymbol::Type::kInt) {
            symbol.set_value((double) back.int_value());
        } else if (back.type() == StackSymbol::Type::kReal) {
            symbol.set_value((double) back.real_value());
        } else {
            throw simulator_error(eip(), "无法取出栈顶元素");
        }
    } else {
        throw simulator_error(eip(), "错误的目标类型");
    }

    symbol.set_assigned();

    inc_eip();
}

void Simulator::pop_array(const PCode &code) {
    StackSymbol back = stack_.back();
    stack_.pop_back();
    Symbol &symbol = tree_.resolve(code.first());

    if (symbol.type() == Symbol::Type::kIntArray) {
        std::vector<int> array = symbol.int_array();
        if (back.type() == StackSymbol::Type::kInt) {
            array[std::stoi(code.second())] = (int) back.int_value();
        } else if (back.type() == StackSymbol::Type::kReal) {
            array[std::stoi(code.second())] = (int) back.real_value();
        } else {
            throw simulator_error(eip(), "无法取出栈顶元素");
        }
        symbol.set_value(array);
    } else if (symbol.type() == Symbol::Type::kRealArray) {
        std::vector<double> array = symbol.real_array();
        if (back.type() == StackSymbol::Type::kInt) {
            array[std::stoi(code.second())] = (double) back.int_value();
        } else if (back.type() == StackSymbol::Type::kReal) {
            array[std::stoi(code.second())] = (double) back.real_value();
        } else {
            throw simulator_error(eip(), "无法取出栈顶元素");
        }
        symbol.set_value(array);
    } else {
        throw simulator_error(eip(), "错误的目标类型");
    }

    symbol.set_assigned();

    inc_eip();
}

void Simulator::add(const PCode &code) {
    StackSymbol first = stack_.back();
    stack_.pop_back();
    StackSymbol second = stack_.back();
    stack_.pop_back();

    double result;
    if (first.type() == StackSymbol::Type::kInt) {
        result = first.int_value();
    } else if (first.type() == StackSymbol::Type::kReal) {
        result = first.real_value();
    } else {
        throw simulator_error(eip(), "不合法的加法操作数");
    }
    if (second.type() == StackSymbol::Type::kInt) {
        result += second.int_value();
    } else if (second.type() == StackSymbol::Type::kReal) {
        result += second.real_value();
    } else {
        throw simulator_error(eip(), "不合法的加法操作数");
    }

    stack_.push_back(StackSymbol(result));
    inc_eip();
}

void Simulator::sub(const PCode &code) {
    StackSymbol first = stack_.back();
    stack_.pop_back();
    StackSymbol second = stack_.back();
    stack_.pop_back();

    double result;
    if (second.type() == StackSymbol::Type::kInt) {
        result = second.int_value();
    } else if (second.type() == StackSymbol::Type::kReal) {
        result = second.real_value();
    } else {
        throw simulator_error(eip(), "不合法的减法操作数");
    }
    if (first.type() == StackSymbol::Type::kInt) {
        result -= first.int_value();
    } else if (first.type() == StackSymbol::Type::kReal) {
        result -= first.real_value();
    } else {
        throw simulator_error(eip(), "不合法的减法操作数");
    }

    stack_.push_back(StackSymbol(result));
    inc_eip();
}

void Simulator::mul(const PCode &code) {
    StackSymbol first = stack_.back();
    stack_.pop_back();
    StackSymbol second = stack_.back();
    stack_.pop_back();

    double result;
    if (second.type() == StackSymbol::Type::kInt) {
        result = second.int_value();
    } else if (second.type() == StackSymbol::Type::kReal) {
        result = second.real_value();
    } else {
        throw simulator_error(eip(), "不合法的乘法操作数");
    }
    if (first.type() == StackSymbol::Type::kInt) {
        result *= first.int_value();
    } else if (first.type() == StackSymbol::Type::kReal) {
        result *= first.real_value();
    } else {
        throw simulator_error(eip(), "不合法的乘法操作数");
    }

    stack_.push_back(StackSymbol(result));
    inc_eip();
}

void Simulator::divide(const PCode &code) {
    StackSymbol first = stack_.back();
    stack_.pop_back();
    StackSymbol second = stack_.back();
    stack_.pop_back();

    double result;
    if (second.type() == StackSymbol::Type::kInt) {
        result = (double)second.int_value();
    } else if (second.type() == StackSymbol::Type::kReal) {
        result = (double)second.real_value();
    } else {
        throw simulator_error(eip(), "不合法的除法操作数");
    }
    if (first.type() == StackSymbol::Type::kInt) {
        if (first.int_value() == 0) {
            throw simulator_error(eip(), "除数不能为 0");
        }
        result /= (double)first.int_value();
    } else if (first.type() == StackSymbol::Type::kReal) {
        if (std::fabs(first.real_value()) < 1e-8) {
            throw simulator_error(eip(), "除数不能为 0");
        }
        result /= (double)first.real_value();
    } else {
        throw simulator_error(eip(), "不合法的除法操作数");
    }

    stack_.push_back(StackSymbol(result));
    inc_eip();
}

void Simulator::mod(const PCode &code) {
    StackSymbol first = stack_.back();
    stack_.pop_back();
    StackSymbol second = stack_.back();
    stack_.pop_back();

    double result;
    if (second.type() == StackSymbol::Type::kInt) {
        result = (double)second.int_value();
    } else if (second.type() == StackSymbol::Type::kReal) {
        result = (double)second.real_value();
    } else {
        throw simulator_error(eip(), "不合法的求余操作数");
    }
    if (first.type() == StackSymbol::Type::kInt) {
        if (first.int_value() == 0) {
            throw simulator_error(eip(), "mod 除数不能为 0");
        }
        result = std::fmod(result, (double)first.int_value());
    } else if (first.type() == StackSymbol::Type::kReal) {
        if (std::fabs(first.real_value()) < 1e-8) {
            throw simulator_error(eip(), "mod 除数不能为 0");
        }
        result = std::fmod(result, (double)first.real_value());
    } else {
        throw simulator_error(eip(), "不合法的求余操作数");
    }

    stack_.push_back(StackSymbol(result));
    inc_eip();
}

void Simulator::compare_equal(const PCode &code) {
    StackSymbol right = stack_.back();
    stack_.pop_back();
    StackSymbol left = stack_.back();
    stack_.pop_back();

    if (left.type() == StackSymbol::Type::kInt) {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.int_value() == right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)((double)left.int_value() == right.real_value())));
        }
    } else {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.real_value() == (double)right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)(left.real_value() == right.real_value())));
        }
    }

    inc_eip();
}

void Simulator::compare_not_equal(const PCode &code) {
    StackSymbol right = stack_.back();
    stack_.pop_back();
    StackSymbol left = stack_.back();
    stack_.pop_back();

    if (left.type() == StackSymbol::Type::kInt) {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.int_value() != right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)((double)left.int_value() != right.real_value())));
        }
    } else {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.real_value() != (double)right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)(left.real_value() != right.real_value())));
        }
    }

    inc_eip();
}

void Simulator::compare_greater_than(const PCode &code) {
    StackSymbol right = stack_.back();
    stack_.pop_back();
    StackSymbol left = stack_.back();
    stack_.pop_back();

    if (left.type() == StackSymbol::Type::kInt) {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.int_value() > right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)((double)left.int_value() > right.real_value())));
        }
    } else {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.real_value() > (double)right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)(left.real_value() > right.real_value())));
        }
    }

    inc_eip();
}

void Simulator::compare_less_than(const PCode &code) {
    StackSymbol right = stack_.back();
    stack_.pop_back();
    StackSymbol left = stack_.back();
    stack_.pop_back();

    if (left.type() == StackSymbol::Type::kInt) {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.int_value() < right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)((double)left.int_value() < right.real_value())));
        }
    } else {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.real_value() < (double)right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)(left.real_value() < right.real_value())));
        }
    }

    inc_eip();
}

void Simulator::compare_greater_equal(const PCode &code) {
    StackSymbol right = stack_.back();
    stack_.pop_back();
    StackSymbol left = stack_.back();
    stack_.pop_back();

    if (left.type() == StackSymbol::Type::kInt) {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.int_value() >= right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)((double)left.int_value() >= right.real_value())));
        }
    } else {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.real_value() >= (double)right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)(left.real_value() >= right.real_value())));
        }
    }

    inc_eip();
}

void Simulator::compare_less_equal(const PCode &code) {
    StackSymbol right = stack_.back();
    stack_.pop_back();
    StackSymbol left = stack_.back();
    stack_.pop_back();

    if (left.type() == StackSymbol::Type::kInt) {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.int_value() <= right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)((double)left.int_value() <= right.real_value())));
        }
    } else {
        if (right.type() == StackSymbol::Type::kInt) {
            stack_.push_back(StackSymbol((int)(left.real_value() <= (double)right.int_value())));
        } else {
            stack_.push_back(StackSymbol((int)(left.real_value() <= right.real_value())));
        }
    }

    inc_eip();
}

void Simulator::jump(const PCode &code) {
    int target = label_table_[code.first()];
    set_eip(target);
}

void Simulator::jump_zero(const PCode &code) {
    int target = label_table_[code.first()];
    StackSymbol symbol = stack_.back();
    stack_.pop_back();
    if (symbol.int_value() == 0) {
        set_eip(target);
    } else {
        inc_eip();
    }
}

void Simulator::jump_not_zero(const PCode &code) {
    int target = label_table_[code.first()];
    StackSymbol symbol = stack_.back();
    stack_.pop_back();
    if (symbol.int_value() != 0) {
        set_eip(target);
    } else {
        inc_eip();
    }
}

void Simulator::label(const PCode &code) {
    std::string name = code.first();

    if (name.substr(0, 7) == "_begin_") {
        if (inloop_) {
            tree_.pop();
        }
        tree_.push();
        inloop_ = true;
    } else if (name.substr(0, 5) == "_end_") {
        tree_.pop();
        inloop_ = false;
    } else if (name.substr(0, 6) == "_else_") {
        tree_.pop();
        tree_.push();
    }

    inc_eip();
}

void Simulator::print(const PCode &code) {
    StackSymbol symbol = stack_.back();
    stack_.pop_back();

    if (symbol.type() == StackSymbol::Type::kInt) {
        std::cout << symbol.int_value() << std::endl;
    } else if (symbol.type() == StackSymbol::Type::kReal) {
        std::cout << symbol.real_value() << std::endl;
    } else {
        throw simulator_error(eip(), "不合法的输出参数");
    }

    inc_eip();
}

void Simulator::read_int(const PCode &code) {
    int input = 0;
    std::cin >> input;
    Symbol &symbol = tree_.resolve(code.first());
    symbol.set_value(input);
    symbol.set_assigned();

    inc_eip();
}

void Simulator::read_int_array(const PCode &code) {
    int input = 0;
    Symbol &symbol = tree_.resolve(code.first());

    std::cin >> input;
    std::vector<int> array = symbol.int_array();
    array[get_second_parameter(code.second())] = input;
    symbol.set_value(array);
    symbol.set_assigned();

    inc_eip();
}

void Simulator::read_real(const PCode &code) {
    double input = 0.0;
    std::cin >> input;
    Symbol &symbol = tree_.resolve(code.first());
    symbol.set_value(input);
    symbol.set_assigned();

    inc_eip();
}

void Simulator::read_real_array(const PCode &code) {
    double input = 0;
    Symbol &symbol = tree_.resolve(code.first());

    std::cin >> input;
    std::vector<double> array = symbol.real_array();
    array[get_second_parameter(code.second())] = input;
    symbol.set_value(array);
    symbol.set_assigned();

    inc_eip();
}

void Simulator::exit_program(const PCode &code) {
    inc_eip();
}

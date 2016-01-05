#ifndef CMM_ICODE_H
#define CMM_ICODE_H

#include <vector>
#include <string>

enum class OpCode {
    kAdd,
    kSub,
    kMul,
    kDiv,

    kGT,
    kGTE,
    kLT,
    kLTE,
    kEqual,
    kNotEqual,

    kVarInteger,
    kVarReal,

    kJmp,
    kJmpZero,
    kJmpNotZero,

    kAssign,
    kRead,
    kWrite,
    kExit,

    kFunc,
    kArg,
    kReturn,
    kCall,
};

struct Var {
    enum class Type { kNone = 0, kInt, kReal, kTemp, kLabel };

    Var() : type(Type::kNone), int_value(0), real_value(0.0), temp_var(0), label("") { }

    Var(Type type, int value) : type(type), int_value(0), real_value(0.0), temp_var(0), label("") {
        if (type == Type::kInt) {
            int_value = value;
        } else {
            temp_var = value;
        }
    }

    Var(Type type, double value) : type(type), int_value(0), real_value(value), temp_var(0), label("") { }

    Var(Type type, std::string value) : type(type), int_value(0), real_value(0.0), temp_var(0), label(value) { }

    friend std::ostream &operator << (std::ostream &os, const Var &var) {
        if (var.type == Type::kInt) {
            os << var.int_value;
        } else if (var.type == Type::kReal) {
            os << var.real_value;
        } else if (var.type == Type::kTemp) {
            os << "T" << var.temp_var;
        } else if (var.type == Type::kLabel) {
            os << var.label;
        }
        return os;
    }

    Type type;
    int int_value;
    double real_value;
    int temp_var;
    std::string label;
};

class IR {
public:
    IR(OpCode op, Var dest, Var src1, Var src2) : op(op), dest(dest), src1(src1), src2(src2) { }

    static const char *ir_type_name(const IR &ir) {
        switch (ir.op) {
            case OpCode::kAdd: return "+";
            case OpCode::kSub: return "-";
            case OpCode::kMul: return "*";
            case OpCode::kDiv: return "/";
            default: throw std::invalid_argument("received invalid IR type value");
        }
    }

    friend std::ostream &operator << (std::ostream &os, const IR &ir) {
        os << "(" << ir_type_name(ir) << ", " << ir.dest << ", " << ir.src1 << ", " << ir.src2 << ")";
        return os;
    }

    OpCode op;
    Var dest;
    Var src1;
    Var src2;
};

#endif //CMM_ICODE_H

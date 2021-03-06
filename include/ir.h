#ifndef CMM_ICODE_H
#define CMM_ICODE_H

#include <vector>
#include <string>

class PCode {
public:
    enum class Type {
        kNone = 0,
        kStartFunc,                      // FUNC @identity:
        kArgInteger,                     // argi a
        kArgIntegerArray,                // argia a
        kArgReal,                        // argr a
        kArgRealArray,                   // argra a
        kReturn,                         // ret
        kEndFunc,                        // ENDFUNC
        kCall,                           // $sum

        kLabel,                          // Label1:

        kVarInteger,                     // vari a
        kVarIntegerArray,                // varia a, b || varia a, 5
        kVarReal,                        // varr b
        kVarRealArray,                   // varra a, b || varra a, 5

        kPushInteger,                    // pushi a || pushi 5
        kPushIntegerArray,               // pushia a, 5
        kPushReal,                       // pushr a || pushr 6.0
        kPushRealArray,                  // pushra a, 5

        kPop,                            // pop
        kPopInteger,                     // popi a
        kPopIntegerArray,                // popia a, 6
        kPopReal,                        // popr a
        kPopRealArray,                   // popra a, 6

        kAdd,                            // add
        kSub,                            // sub
        kMul,                            // mul
        kDiv,                            // div
        kMod,                            // mod

        kCompareEqual,                   // cmpeq
        kCompareNotEqual,                // cmpne
        kCompareGreaterThan,             // cmpgt
        kCompareLessThan,                // cmplt
        kCompareGreaterEqual,            // cmpge
        kCompareLessEqual,               // cmple

        kAnd,                            // and
        kOr,                             // or
        kNot,                            // not
        kNegative,                       // neg

        kJump,                           // jmp
        kJumpZero,                       // jz
        kJumpNotZero,                    // jnz

        kPrint,                          // print
        kReadInt,                        // readi
        kReadIntArray,                   // readia
        kReadReal,                       // readr
        kReadRealArray,                  // readra

        kExit,                           // exit
    };

    PCode(const int &indent = 4) : type_(Type::kNone), indent_(indent) { }

    PCode(const Type &type, const int &indent = 4) : type_(type), indent_(indent) { }

    PCode(const Type &type, const std::string &first, const int &indent = 4) : type_(type), first_(first), indent_(indent) { }

    PCode(const Type &type, const std::string &first, const std::string &second, const int &indent = 4) : type_(type), first_(first), second_(second), indent_(indent) { }

    PCode(const Type &type, const std::string &first, const std::string &second, const std::string &third, const int &indent = 4) : type_(type), first_(first), second_(second), third_(third), indent_(indent) { }

    const Type &type() const {
        return type_;
    }

    const std::string &first() const {
        return first_;
    }

    const std::string &second() const {
        return second_;
    }

    const std::string &third() const {
        return third_;
    }

    int indent() const {
        return indent_;
    }

    void set(const Type &type) {
        type_ = type;
    }

    void set(const Type &type, const std::string &first) {
        type_ = type;
        first_ = first;
    }

    void set(const Type &type, const std::string &first, const std::string &second) {
        type_ = type;
        first_ = first;
        second_ = second;
    }

    void set(const Type &type, const std::string &first, const std::string &second, const std::string &third) {
        type_ = type;
        first_ = first;
        second_ = second;
        third_ = third;
    }

    void set_indent(const int &indent) {
        indent_ = indent;
    }

    static std::ostream &print_indent(std::ostream &os, const PCode &pcode) {
        if (pcode.type() != Type::kStartFunc && pcode.type() != Type::kEndFunc && pcode.type() != Type::kLabel) {
            for (int i = 0; i < pcode.indent(); ++i) {
                os << " ";
            }
        }
        return os;
    }

    friend std::ostream &operator << (std::ostream &os, const PCode &pcode) {
        pcode.print_indent(os, pcode);
        switch (pcode.type()) {
            case Type::kNone:
                os << "none";
                break;
            case Type::kStartFunc:
                os << "FUNC @" << pcode.first() << ":";
                break;
            case Type::kArgInteger:
                os << "argi " << pcode.first();
                break;
            case Type::kArgIntegerArray:
                os << "argia " << pcode.first();
                break;
            case Type::kArgReal:
                os << "argr " << pcode.first();
                break;
            case Type::kArgRealArray:
                os << "argra " << pcode.first();
                break;
            case Type::kReturn:
                os << "ret";
                break;
            case Type::kEndFunc:
                os << "ENDFUNC";
                break;
            case Type::kCall:
                os << "$" << pcode.first();
                break;
            case Type::kLabel:
                os << pcode.first() << ":";
                break;
            case Type::kVarInteger:
                os << "vari " << pcode.first();
                break;
            case Type::kVarIntegerArray:
                os << "varia " << pcode.first() << ", " << pcode.second();
                break;
            case Type::kVarReal:
                os << "varr " << pcode.first();
                break;
            case Type::kVarRealArray:
                os << "varra " << pcode.first() << ", " << pcode.second();
                break;
            case Type::kPushInteger:
                os << "pushi " << pcode.first();
                break;
            case Type::kPushIntegerArray:
                os << "pushia " << pcode.first() << ", " << pcode.second();
                break;
            case Type::kPushReal:
                os << "pushr " << pcode.first();
                break;
            case Type::kPushRealArray:
                os << "pushra " << pcode.first() << ", " << pcode.second();
                break;
            case Type::kPop:
                os << "pop";
                break;
            case Type::kPopInteger:
                os << "pop_id " << pcode.first();
                break;
            case Type::kPopIntegerArray:
                os << "pop_arr " << pcode.first() << ", " << pcode.second();
                break;
            case Type::kPopReal:
                os << "pop_id " << pcode.first();
                break;
            case Type::kPopRealArray:
                os << "pop_arr " << pcode.first() << ", " << pcode.second();
                break;
            case Type::kAdd:
                os << "add";
                break;
            case Type::kSub:
                os << "sub";
                break;
            case Type::kMul:
                os << "mul";
                break;
            case Type::kDiv:
                os << "div";
                break;
            case Type::kMod:
                os << "mod";
                break;
            case Type::kCompareEqual:
                os << "cmpeq";
                break;
            case Type::kCompareNotEqual:
                os << "cmpne";
                break;
            case Type::kCompareGreaterThan:
                os << "cmpgt";
                break;
            case Type::kCompareLessThan:
                os << "cmplt";
                break;
            case Type::kCompareGreaterEqual:
                os << "cmpge";
                break;
            case Type::kCompareLessEqual:
                os << "cmple";
                break;
            case Type::kAnd:
                os << "and";
                break;
            case Type::kOr:
                os << "or";
                break;
            case Type::kNot:
                os << "not";
                break;
            case Type::kNegative:
                os << "neg";
                break;
            case Type::kJump:
                os << "jmp " << pcode.first();
                break;
            case Type::kJumpZero:
                os << "jz " << pcode.first();
                break;
            case Type::kJumpNotZero:
                os << "jnz " << pcode.first();
                break;
            case Type::kPrint:
                os << "print";
                break;
            case Type::kReadInt:
                os << "readi " << pcode.first();
                break;
            case Type::kReadIntArray:
                os << "readia " << pcode.first() << ", " << pcode.second();
                break;
            case Type::kReadReal:
                os << "readr " << pcode.first();
                break;
            case Type::kReadRealArray:
                os << "readra " << pcode.first() << ", " << pcode.second();
                break;
            case Type::kExit:
                os << "exit " << pcode.first();
                break;
        }
        return os;
    }

private:
    Type type_;
    std::string first_;
    std::string second_;
    std::string third_;
    int indent_;
};

class IR {
public:
    IR() {}

    void add(const PCode &pcode) {
        ir_.push_back(pcode);
    }

    const PCode &at(int pos) const {
        return ir_.at((unsigned long)pos);
    }

    int size() const {
        return (int)ir_.size();
    }

    friend std::ostream &operator << (std::ostream &os, const IR &ir) {
        for (int i = 0; i < ir.size(); ++i) {
            os << i << ":\t\t| " << ir.at(i) << std::endl;
        }
        return os;
    }

private:
    std::vector<PCode> ir_;
};

#endif //CMM_ICODE_H

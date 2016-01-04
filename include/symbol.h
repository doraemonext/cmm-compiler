#ifndef CMM_SYMBOL_H
#define CMM_SYMBOL_H

#include <iostream>
#include <vector>
#include <string>

class Symbol {
public:
    enum Type {
        kNone = 0,
        kMethod,
        kInt,
        kIntArray,
        kReal,
        kRealArray,
        kVoid,
    };

    // 针对函数的构造函数, 默认构造返回值为 void, 参数列表为空
    Symbol(const std::string &name, const Type &ret_type = Symbol::Type::kVoid, const std::vector<Symbol> &args = {}) :
            name_(name), type_(Symbol::Type::kMethod), ret_type_(ret_type) {
        value_.args = args;
    }

    // 针对 int 的构造函数
    Symbol(const std::string &name, const int &value) :
            name_(name), type_(Symbol::Type::kInt), ret_type_(Symbol::Type::kNone) {
        value_.int_value = value;
    }

    // 针对 int_array 的构造函数
    Symbol(const std::string &name, const std::vector<int> &value) :
            name_(name), type_(Symbol::Type::kIntArray), ret_type_(Symbol::Type::kNone) {
        value_.int_array = value;
    }

    // 针对 real 的构造函数
    Symbol(const std::string &name, const double &value) :
            name_(name), type_(Symbol::Type::kReal), ret_type_(Symbol::Type::kNone) {
        value_.real_value = value;
    }

    // 针对 real_array 的构造函数
    Symbol(const std::string &name, const std::vector<double> &value) :
            name_(name), type_(Symbol::Type::kRealArray), ret_type_(Symbol::Type::kNone) {
        value_.real_array = value;
    }

    std::string name() const {
        return name_;
    }

    void set_name(const std::string &name) {
        name_ = name;
    }

    Type type() const {
        return type_;
    }

    void set_type(const Type &type) {
        type_ = type;
    }

    Type ret_type() const {
        return ret_type_;
    }

    void set_ret_type(const Type &ret_type) {
        ret_type_ = ret_type;
    }

    const std::vector<Symbol> &args() const {
        return value_.args;
    }

    const int &int_value() const {
        return value_.int_value;
    }

    const std::vector<int> &int_array() const {
        return value_.int_array;
    }

    const double &real_value() const {
        return value_.real_value;
    }

    const std::vector<double> &real_array() const {
        return value_.real_array;
    }

    void set_value(const std::vector<Symbol> &value) {
        value_.args = value;
    }

    void set_value(const int &value) {
        value_.int_value = value;
    }

    void set_value(const std::vector<int> &value) {
        value_.int_array = value;
    }

    void set_value(const double &value) {
        value_.real_value = value;
    }

    void set_value(const std::vector<double> &value) {
        value_.real_array = value;
    }

    // 测试函数
    void print() const;

private:
    typedef struct {
        std::vector<Symbol> args;
        int int_value;
        std::vector<int> int_array;
        double real_value;
        std::vector<double> real_array;
    } UnionValue;

    std::string name_;
    Type type_;
    Type ret_type_;
    UnionValue value_;
};

#endif //CMM_SYMBOL_H

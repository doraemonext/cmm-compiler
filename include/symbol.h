#ifndef CMM_SYMBOL_H
#define CMM_SYMBOL_H

#include <iostream>
#include <vector>
#include <string>
#include "token.h"

class Symbol {
public:
    enum class Type {
        kNone = 0,
        kFunction,
        kInt,
        kIntArray,
        kReal,
        kRealArray,
        kVoid,
    };

    Symbol() : name_(""), type_(Symbol::Type::kNone), ret_type_(Symbol::Type::kNone), is_assigned_(false) { }

    // 针对函数的构造函数, 默认构造返回值为 void, 参数列表为空
    Symbol(const std::string &name, const Type &ret_type = Symbol::Type::kVoid, const std::vector<Symbol> &args = {}) :
            name_(name), type_(Symbol::Type::kFunction), ret_type_(ret_type), is_assigned_(true) {
        value_.args = args;
    }

    // 针对 int 的构造函数
    Symbol(const std::string &name, const int &value, const bool &is_assigned) :
            name_(name), type_(Symbol::Type::kInt), ret_type_(Symbol::Type::kNone), is_assigned_(is_assigned) {
        value_.int_value = value;
    }

    // 针对 int_array 的构造函数
    Symbol(const std::string &name, const std::vector<int> &value, const bool &is_assigned) :
            name_(name), type_(Symbol::Type::kIntArray), ret_type_(Symbol::Type::kNone), is_assigned_(true) {
        value_.int_array = value;
    }

    // 针对 real 的构造函数
    Symbol(const std::string &name, const double &value, const bool &is_assigned) :
            name_(name), type_(Symbol::Type::kReal), ret_type_(Symbol::Type::kNone), is_assigned_(is_assigned) {
        value_.real_value = value;
    }

    // 针对 real_array 的构造函数
    Symbol(const std::string &name, const std::vector<double> &value, const bool &is_assigned) :
            name_(name), type_(Symbol::Type::kRealArray), ret_type_(Symbol::Type::kNone), is_assigned_(true) {
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

    const char *type_name() const {
        return symbol_type_name(type_);
    }

    void set_type(const Type &type) {
        type_ = type;
    }

    Type ret_type() const {
        return ret_type_;
    }

    const char *ret_type_name() const {
        return symbol_type_name(ret_type_);
    }

    void set_ret_type(const Type &ret_type) {
        ret_type_ = ret_type;
    }

    bool is_assigned() const {
        return is_assigned_;
    }

    void set_assigned() {
        is_assigned_ = true;
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
        is_assigned_ = true;
    }

    void set_value(const int &value) {
        value_.int_value = value;
        is_assigned_ = true;
    }

    void set_value(const std::vector<int> &value) {
        value_.int_array = value;
        is_assigned_ = true;
    }

    void set_value(const double &value) {
        value_.real_value = value;
        is_assigned_ = true;
    }

    void set_value(const std::vector<double> &value) {
        value_.real_array = value;
        is_assigned_ = true;
    }

    static const Type convert_token_type(const Token::Type &token_type) {
        switch (token_type) {
            case Token::Type::kFunction:
                return Type::kFunction;
            case Token::Type::kInt:
                return Type::kInt;
            case Token::Type::kReal:
                return Type::kReal;
            case Token::Type::kIntArray:
                return Type::kIntArray;
            case Token::Type::kRealArray:
                return Type::kRealArray;
            case Token::Type::kVoid:
                return Type::kVoid;
            default:
                throw std::invalid_argument("cannot convert to Symbol::Type from Token::Type with received parameter");
        }
    }

    static const Token::Type convert_symbol_type(const Type &symbol_type) {
        switch (symbol_type) {
            case Type::kFunction:
                return Token::Type::kFunction;
            case Type::kInt:
                return Token::Type::kInt;
            case Type::kReal:
                return Token::Type::kReal;
            case Type::kIntArray:
                return Token::Type::kIntArray;
            case Type::kRealArray:
                return Token::Type::kRealArray;
            case Type::kVoid:
                return Token::Type::kVoid;
            default:
                throw std::invalid_argument("cannot convert to Token::Type from Symbol::Type with received parameter");
        }
    }

    static const char *symbol_type_name(const Type &type) {
        switch (type) {
            case Type::kNone: return "none";
            case Type::kFunction: return "function";
            case Type::kInt: return "int";
            case Type::kIntArray: return "int_array";
            case Type::kReal: return "real";
            case Type::kRealArray: return "real_array";
            case Type::kVoid: return "void";
        }
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
    bool is_assigned_;
};

class StackSymbol {
public:
    enum class Type {
        kInt = 0,
        kIntArray,
        kReal,
        kRealArray,
    };

    // 针对 int 的构造函数
    StackSymbol(const int &value) : type_(Symbol::Type::kInt) {
        value_.int_value = value;
    }

    // 针对 int_array 的构造函数
    StackSymbol(const std::vector<int> &value) : type_(Symbol::Type::kIntArray) {
        value_.int_array = value;
    }

    // 针对 real 的构造函数
    StackSymbol(const double &value) : type_(Symbol::Type::kReal) {
        value_.real_value = value;
    }

    // 针对 real_array 的构造函数
    StackSymbol(const std::vector<double> &value) : type_(Symbol::Type::kRealArray) {
        value_.real_array = value;
    }

    Type type() const {
        return type_;
    }

    const char *type_name() const {
        return symbol_type_name(type_);
    }

    void set_type(const Type &type) {
        type_ = type;
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

    static const Type convert_token_type(const Token::Type &token_type) {
        switch (token_type) {
            case Token::Type::kInt:
                return Type::kInt;
            case Token::Type::kReal:
                return Type::kReal;
            case Token::Type::kIntArray:
                return Type::kIntArray;
            case Token::Type::kRealArray:
                return Type::kRealArray;
            default:
                throw std::invalid_argument("cannot convert to Symbol::Type from Token::Type with received parameter");
        }
    }

    static const Token::Type convert_symbol_type(const Type &symbol_type) {
        switch (symbol_type) {
            case Type::kInt:
                return Token::Type::kInt;
            case Type::kReal:
                return Token::Type::kReal;
            case Type::kIntArray:
                return Token::Type::kIntArray;
            case Type::kRealArray:
                return Token::Type::kRealArray;
        }
    }

    static const char *symbol_type_name(const Type &type) {
        switch (type) {
            case Type::kInt: return "int";
            case Type::kIntArray: return "int_array";
            case Type::kReal: return "real";
            case Type::kRealArray: return "real_array";
        }
    }

private:
    typedef struct {
        int int_value;
        std::vector<int> int_array;
        double real_value;
        std::vector<double> real_array;
    } UnionValue;

    Type type_;
    UnionValue value_;
};

#endif //CMM_SYMBOL_H


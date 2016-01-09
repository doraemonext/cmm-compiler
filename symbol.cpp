#include "include/symbol.h"

// 测试函数
void Symbol::print() const {
    std::cout << "Name: " << name_ << std::endl;
    if (type_ == Symbol::Type::kInt) {
        std::cout << "Int Value: " << value_.int_value << std::endl;
        std::cout << "Is Assigned: " << is_assigned_ << std::endl;
    } else if (type_ == Symbol::Type::kIntArray) {
        std::cout << "Int Array: ";
        for (int i = 0; i < (int)value_.int_array.size(); i++) {
            std::cout << value_.int_array[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Is Assigned: " << is_assigned_ << std::endl;
    } else if (type_ == Symbol::Type::kReal) {
        std::cout << "Real Value: " << value_.real_value << std::endl;
        std::cout << "Is Assigned: " << is_assigned_ << std::endl;
    } else if (type_ == Symbol::Type::kRealArray) {
        std::cout << "Real Array: ";
        for (int i = 0; i < (int)value_.real_array.size(); i++) {
            std::cout << value_.real_array[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Is Assigned: " << is_assigned_ << std::endl;
    } else if (type_ == Symbol::Type::kFunction) {
        std::cout << "Method Args: " << std::endl;
        for (int i = 0; i < (int)value_.args.size(); i++) {
            std::cout << "    Arg " << i + 1 << ":" << std::endl;
            std::cout << "        Name: " << value_.args[i].name_ << std::endl;
            if (value_.args[i].type_ == Symbol::Type::kInt) {
                std::cout << "        Int Value: " << value_.args[i].value_.int_value << std::endl;
                std::cout << "        Is Assigned: " << value_.args[i].is_assigned_ << std::endl;
            } else if (value_.args[i].type_ == Symbol::Type::kIntArray) {
                std::cout << "        Int Array: ";
                for (int j = 0; j < (int)value_.args[j].value_.int_array.size(); j++) {
                    std::cout << value_.args[j].value_.int_array[j] << " ";
                }
                std::cout << std::endl;
                std::cout << "        Is Assigned: " << value_.args[i].is_assigned_ << std::endl;
            } else if (value_.args[i].type_ == Symbol::Type::kReal) {
                std::cout << "        Real Value: " << value_.args[i].value_.real_value << std::endl;
                std::cout << "        Is Assigned: " << value_.args[i].is_assigned_ << std::endl;
            } else if (value_.args[i].type_ == Symbol::Type::kRealArray) {
                std::cout << "        Real Array: ";
                for (int j = 0; i < (int) value_.args[j].value_.real_array.size(); j++) {
                    std::cout << value_.args[j].value_.real_array[j] << " ";
                }
                std::cout << std::endl;
                std::cout << "        Is Assigned: " << value_.args[i].is_assigned_ << std::endl;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "===================================" << std::endl;
}

// 测试函数
void StackSymbol::print() const {
    if (type_ == StackSymbol::Type::kInt) {
        std::cout << "Int Value: " << value_.int_value << std::endl;
    } else if (type_ == StackSymbol::Type::kIntArray) {
        std::cout << "Int Array: ";
        for (int i = 0; i < (int)value_.int_array.size(); i++) {
            std::cout << value_.int_array[i] << " ";
        }
        std::cout << std::endl;
    } else if (type_ == StackSymbol::Type::kReal) {
        std::cout << "Real Value: " << value_.real_value << std::endl;
    } else if (type_ == StackSymbol::Type::kRealArray) {
        std::cout << "Real Array: ";
        for (int i = 0; i < (int)value_.real_array.size(); i++) {
            std::cout << value_.real_array[i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "===================================" << std::endl;
}
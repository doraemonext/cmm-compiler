#include "include/symbol.h"

// 测试函数
void Symbol::print() const {
    std::cout << "Name: " << name_ << std::endl;
    if (type_ == Symbol::Type::kInt) {
        std::cout << "Int Value: " << value_.int_value << std::endl;
    } else if (type_ == Symbol::Type::kIntArray) {
        std::cout << "Int Array: ";
        for (int i = 0; i < (int)value_.int_array.size(); i++) {
            std::cout << value_.int_array[i] << " ";
        }
        std::cout << std::endl;
    } else if (type_ == Symbol::Type::kReal) {
        std::cout << "Real Value: " << value_.real_value << std::endl;
    } else if (type_ == Symbol::Type::kRealArray) {
        std::cout << "Real Array: ";
        for (int i = 0; i < (int)value_.real_array.size(); i++) {
            std::cout << value_.real_array[i] << " ";
        }
        std::cout << std::endl;
    } else if (type_ == Symbol::Type::kMethod) {
        std::cout << "Method Args: ";
        for (int i = 0; i < (int)value_.args.size(); i++) {
            std::cout << value_.args[i].name_ << " ";
        }
        std::cout << std::endl;
    }
}
#ifndef CMM_SEMANTIC_H
#define CMM_SEMANTIC_H

#include <iostream>
#include <vector>
#include "ast.h"
#include "symbol.h"
#include "scope.h"
#include "ir.h"

class Semantic {
public:
    Semantic(AbstractSyntaxNode *root) : ir_indent_(4) {
        root_ = root;
        current_ = root_;
    }

    void analyse() {
        analyse_program();
    }

    Token analyse_program() {
        for (int i = 0; i < children_size(); ++i) {
            if (child_type(i) == Token::Type::kFunction) {
                analyse_function(i);
            } else if (child_type(i) == Token::Type::kStatement) {
                analyse_statement(i, Symbol("__global__"));
            }
        }
        return Token(Token::Type::kProgram, current_->token().position());
    }

    // 分析函数
    Token analyse_function(const int &pos) {
        current_ = child(pos);

        int offset = 0;
        Token declare_keyword = analyse_declare_keyword(offset++);
        Token identity = analyse_identity(offset++);

        // 查找重复函数名称
        try {
            tree_.resolve(identity.content());
            add_error_messages(identity.position(), "重复定义的函数名称 \"" + std::string(identity.content()) + "\", 请尝试使用其他不冲突的函数名称");
            throw scope_critical_error();
        } catch (const scope_not_found &e) { }

        // 分析函数形参
        std::vector<std::pair<Token, Token> > parameters;
        if (child_type(offset) == Token::Type::kFunctionParameters) {
            current_ = child(offset++);
            int parameters_offset = 0;
            while (parameters_offset < children_size() && child_type(parameters_offset) == Token::Type::kFunctionParameter) {
                current_ = child(parameters_offset++);
                Token first = analyse_declare_keyword(0);  // 函数形参定义关键字 Token
                Token second = analyse_identity(1);        // 函数形参变量名 Token

                // 检查使用 void 空类型的函数形参
                if (first.type() == Token::Type::kVoid) {
                    add_error_messages(first.position(), "函数形参不能使用空类型 \"void\"");
                    throw scope_critical_error();
                }
                // 检查函数形参是否覆盖上层作用域的同名变量
                try {
                    tree_.resolve(second.content());
                    add_warning_messages(second.position(), "函数 \"" + std::string(identity.content()) + "\" 的形参 \"" + second.content() + "\" 将会覆盖上层作用域的同名变量");
                } catch (const scope_not_found &e) { }

                parameters.push_back(std::pair<Token, Token>(first, second));
                current_ = current_->parent();
            }
            current_ = current_->parent();
        }

        // 向全局作用域树种添加函数名称
        std::vector<Symbol> parameters_symbol;
        for (int i = 0; i < (int)parameters.size(); ++i) {
            const Token &first = parameters[i].first;
            const Token &second = parameters[i].second;
            switch (first.type()) {
                case Token::Type::kInt:
                    parameters_symbol.push_back(Symbol(second.content(), 0, true));
                    break;
                case Token::Type::kIntArray:
                    parameters_symbol.push_back(Symbol(second.content(), std::vector<int>(), true));
                    break;
                case Token::Type::kReal:
                    parameters_symbol.push_back(Symbol(second.content(), 0.0, true));
                    break;
                case Token::Type::kRealArray:
                    parameters_symbol.push_back(Symbol(second.content(), std::vector<double>(), true));
                    break;
                default:
                    break;
            }
        }
        Symbol function_symbol = Symbol(identity.content(), Symbol::convert_token_type(declare_keyword.type()), parameters_symbol);
        tree_.define(function_symbol);
        tree_.push();  // 作用域递增
        for (int i = 0; i < (int)parameters_symbol.size(); ++i) {
            try {
                tree_.define(parameters_symbol[i]);
            } catch (const scope_name_exists &e) {
                add_error_messages(parameters[i].second.position(), "函数 \"" + std::string(identity.content()) + "\" 的形参 \"" + parameters_symbol[i].name() + "\" 重复定义");
                throw scope_critical_error();
            }
        }

        build_function_ir_start(identity);      // 生成函数定义起始位置 IR
        build_function_ir_args(parameters);     // 生成函数形参 IR

        if (offset < children_size() && child_type(offset) == Token::Type::kFunctionStatements) {
            current_ = child(offset++);
            int statements_offset = 0;
            while (statements_offset < children_size() && child_type(statements_offset) == Token::Type::kStatement) {
                analyse_statement(statements_offset++, function_symbol);
            }
            current_ = current_->parent();
        }
        tree_.pop(); // 作用域递减

        build_function_ir_end(identity);        // 生成函数结尾 IR
        current_ = current_->parent();
        return Token(Token::Type::kFunction, current_->token().position());
    }

    // 解析语句
    Token analyse_statement(const int &pos, const Symbol &function_symbol) {
        current_ = child(pos);

        Token result;
        switch (child_type(0)) {
            case Token::Type::kIfStatement:
                analyse_if_statement(0, function_symbol);
                break;
            case Token::Type::kWhileStatement:
                analyse_while_statement(0, function_symbol);
                break;
            case Token::Type::kReadStatement:
                result = analyse_read_statement(0);
                break;
            case Token::Type::kWriteStatement:
                result = analyse_write_statement(0);
                break;
            case Token::Type::kAssignStatement:
                result = analyse_assign_statement(0);
                break;
            case Token::Type::kDeclareStatement:
                result = analyse_declare_statement(0);
                break;
            case Token::Type::kReturnStatement:
                result = analyse_return_statement(0, function_symbol);
                break;
            default:
                break;
        }

        current_ = current_->parent();
        return result;
    }

    // 解析 if 语句
    Token analyse_if_statement(const int &pos, const Symbol &function_symbol) {
        current_ = child(pos);
        Token result;
        std::string if_signature = "_" + std::to_string(current_->token().position().row()) + "_" + std::to_string(current_->token().position().col());   // TODO: 扩展多文件时需要修改

        build_if_statement_ir_begin(if_signature);
        Token condition = analyse_condition(0);
        build_if_statement_ir_jz(if_signature);
        current_ = child(1);
        for (int i = 0; i < children_size(); ++i) {
            analyse_statement(i, function_symbol);
        }
        current_ = current_->parent();
        build_if_statement_ir_else(if_signature);
        if (children_size() > 2) {
            current_ = child(2);
            for (int i = 0; i < children_size(); ++i) {
                analyse_statement(i, function_symbol);
            }
            current_ = current_->parent();
        }
        build_if_statement_ir_end(if_signature);

        current_ = current_->parent();

        return result;
    }

    // 解析 while 语句
    Token analyse_while_statement(const int &pos, const Symbol &function_symbol) {
        current_ = child(pos);
        Token result;
        std::string while_signature = "_" + std::to_string(current_->token().position().row()) + "_" + std::to_string(current_->token().position().col());   // TODO: 扩展多文件时需要修改

        build_while_statement_ir_begin(while_signature);
        Token condition = analyse_condition(0);
        build_while_statement_ir_jz(while_signature);
        current_ = child(1);
        for (int i = 0; i < children_size(); ++i) {
            analyse_statement(i, function_symbol);
        }
        current_ = current_->parent();
        build_while_statement_ir_end(while_signature);

        current_ = current_->parent();
        return result;
    }

    // 解析 read 语句
    Token analyse_read_statement(const int &pos) {
        current_ = child(pos);
        Token result = analyse_identity_array(0);
        Symbol symbol;

        try {
            symbol = tree_.resolve(result.content());
        } catch (const scope_not_found &e) {
            add_error_messages(result.position(), "标识符 \"" + result.content() + "\" 尚未定义, 不能 read");
            throw scope_critical_error();
        }

        if (symbol.type() == Symbol::Type::kFunction) {
            add_error_messages(result.position(), "不能向函数 \"" + symbol.name() + "\" 读入数据");
            throw scope_critical_error();
        }
        if (result.type() == Token::Type::kIdentity && (symbol.type() != Symbol::Type::kInt && symbol.type() != Symbol::Type::kReal)) {
            add_error_messages(result.position(), "仅能向 int 或 real 形式的变量读入数据");
            throw scope_critical_error();
        }
        if (result.type() == Token::Type::kIdentityArray && (symbol.type() != Symbol::Type::kIntArray && symbol.type() != Symbol::Type::kRealArray)) {
            add_error_messages(result.position(), "普通变量 \"" + symbol.name() + "\" 不能使用数组寻址方式");
            throw scope_critical_error();
        }

        build_read_statement_ir(result, symbol);
        tree_.resolve(result.content()).set_assigned();

        current_ = current_->parent();
        return result;
    }

    // 解析 write 语句
    Token analyse_write_statement(const int &pos) {
        current_ = child(pos);
        Token result = analyse_expression(0);
        build_write_statement_ir();
        current_ = current_->parent();
        return result;
    }

    // 解析赋值语句
    Token analyse_assign_statement(const int &pos) {
        current_ = child(pos);

        // 分界左值格式
        int offset = 0;
        Token left_identity = analyse_identity_array(offset++);
        // 检查标识符是否定义
        Symbol left_identity_symbol;
        try {
            left_identity_symbol = tree_.resolve(left_identity.content());
            if (left_identity.type() == Token::Type::kIdentity && (left_identity_symbol.type() == Symbol::Type::kIntArray || left_identity_symbol.type() == Symbol::Type::kRealArray)) {
                add_error_messages(left_identity.position(), "不能直接为数组 \"" + left_identity.content() + "\" 赋值");
                throw scope_critical_error();
            }
            if ((left_identity.type() == Token::Type::kIdentity && left_identity_symbol.type() != Symbol::Type::kInt && left_identity_symbol.type() != Symbol::Type::kReal) ||  // 左值类型为变量, 但符号表中其值并不为 int 或 real 时
                (left_identity.type() == Token::Type::kIdentityArray && left_identity_symbol.type() != Symbol::Type::kIntArray && left_identity_symbol.type() != Symbol::Type::kRealArray)) {  // 左值类型为数组,但符号表中其值并不为 int[] 或 real[] 时
                add_error_messages(left_identity.position(), "左值 \"" + left_identity.content() + "\" 类型与其定义类型不符");
                throw scope_critical_error();
            }
        } catch (const scope_not_found &e) {
            add_error_messages(left_identity.position(), "未定义的标识符 \"" + left_identity.content() + "\" 不能用于赋值语句的左值");
            throw scope_critical_error();
        }

        // 检查数组偏移量是否越界
        if (left_identity.type() == Token::Type::kIntArray || left_identity.type() == Token::Type::kRealArray) {
            if ((left_identity_symbol.type() == Symbol::Type::kIntArray && left_identity_symbol.int_array().size() <= std::stoi(left_identity.extra().at(1))) ||
                (left_identity_symbol.type() == Symbol::Type::kRealArray && left_identity_symbol.real_array().size() <= std::stoi(left_identity.extra().at(1)))) {
                add_error_messages(left_identity.position(), "数组 \"" + left_identity.content() + "\" 的偏移地址越界");
                throw scope_critical_error();
            }
        }

        Token right_expression = analyse_expression(offset++);
        if (right_expression.type() == Token::Type::kReal && (left_identity_symbol.type() == Symbol::Type::kInt || left_identity_symbol.type() == Symbol::Type::kIntArray)) {
            add_error_messages(left_identity.position(), "real 类型无法赋值给左值类型");
            throw scope_critical_error();
        }

        build_assign_statement_ir(left_identity, right_expression);
        tree_.resolve(left_identity.content()).set_assigned();  // 直接修改AST树种的节点

        current_ = current_->parent();
        return Token(Token::Type::kAssignStatement, current_->token().position());
    }

    // 解析定义语句
    Token analyse_declare_statement(const int &pos) {
        current_ = child(pos);

        int offset = 0;
        Token declare_keyword = analyse_declare_keyword(offset++);
        std::vector<Token> identity;
        while (offset < children_size()) {
            identity.push_back(analyse_identity(offset++));

            // 检查是否重复定义
            const Token &last = identity.back();
            try {
                ScopeNode *node = tree_.resolve_scope(last.content());
                if (node->level() == tree_.current()->level()) {
                    add_error_messages(last.position(), "重复定义的变量 \"" + last.content() + "\"");
                    throw scope_critical_error();
                } else {
                    add_warning_messages(last.position(), "重复定义变量 \"" + last.content() + "\" 将会覆盖上层作用域的同名变量");
                }
            } catch (const scope_not_found &e) { }

            // 定义
            switch (declare_keyword.type()) {
                case Token::Type::kInt:
                    tree_.define(Symbol(last.content(), 0, false));
                    break;
                case Token::Type::kIntArray:
                    tree_.define(Symbol(last.content(), std::vector<int>(std::stoul(declare_keyword.content())), false));
                    break;
                case Token::Type::kReal:
                    tree_.define(Symbol(last.content(), 0.0, false));
                    break;
                case Token::Type::kRealArray:
                    tree_.define(Symbol(last.content(), std::vector<double>(std::stoul(declare_keyword.content())), false));
                    break;
                case Token::Type::kVoid:
                    add_error_messages(last.position(), "变量 \"" + last.content() + "\" 类型不合法, 不允许使用 void 类型定义变量");
                    throw scope_critical_error();
                default:
                    break;
            }

            // 生成 IR
            build_declare_statement_ir(declare_keyword, last);
        }

        current_ = current_->parent();
        return Token(Token::Type::kDeclareStatement, identity.back().position());
    }

    // 解析 return 语句
    Token analyse_return_statement(const int &pos, const Symbol &function_symbol) {
        current_ = child(pos);

        Token result = analyse_expression(0);
        if (function_symbol.ret_type() != Symbol::convert_token_type(result.type())) {
            if (result.type() == Token::Type::kInt && function_symbol.type() == Symbol::Type::kReal) {
                result.set_type(Token::Type::kReal);
            } else {
                add_error_messages(result.position(), "函数 \"" + function_symbol.name() + "\" 的返回类型不相符");
                throw scope_critical_error();
            }
        }

        // 生成 IR
        build_return_statement_ir(result);

        current_ = current_->parent();
        return result;
    }

    // 分析定义关键字
    Token analyse_declare_keyword(const int &pos) {
        current_ = child(pos);
        Token result;

        if (current_->children().size() == 1) {
            if (child_type(0) == Token::Type::kInt) {
                result = Token(Token::Type::kInt, child(0)->token().position());
            } else if (child_type(0) == Token::Type::kReal) {
                result = Token(Token::Type::kReal, child(0)->token().position());
            } else if (child_type(0) == Token::Type::kVoid) {
                result = Token(Token::Type::kVoid, child(0)->token().position());
            } else {
                add_error_messages(child(0)->token().position(), "错误的定义类型 \"" + std::string(Token::token_type_name(child_type(0))) + "\"");
                throw scope_critical_error();
            }
        } else {
            if (child(1)->child(0)->token().type() != Token::Type::kIntegerLiteral) {
                add_error_messages(child(0)->token().position(), "错误的数组大小, 仅允许使用整数");
                throw scope_critical_error();
            }

            if (child_type(0) == Token::Type::kInt) {
                result = Token(Token::Type::kIntArray, child(1)->child(0)->token().content(), current_->token().position());
            } else if (child_type(0) == Token::Type::kReal) {
                result = Token(Token::Type::kRealArray, child(1)->child(0)->token().content(), current_->token().position());
            } else {
                add_error_messages(child(0)->token().position(), "错误的定义类型 \"" + std::string(Token::token_type_name(child_type(0))) + "\"");
                throw scope_critical_error();
            }
        }

        current_ = current_->parent();
        return result;
    }

    // 解析带数组偏移的标识符
    Token analyse_identity_array(const int &pos) {
        current_ = child(pos);
        Token result;

        if (current_->children().size() == 1) {
            if (child_type(0) == Token::Type::kIdentity) {
                result = Token(Token::Type::kIdentity, child(0)->token().content(), child(0)->token().position());
            } else {
                add_error_messages(child(0)->token().position(), "错误的值 \"" + child(0)->token().content() + "\"");
                throw scope_critical_error();
            }
        } else {
            Token array = child(1)->child(0)->token();
            if (array.type() != Token::Type::kIntegerLiteral && array.type() != Token::Type::kIdentity) {
                add_error_messages(child(0)->token().position(), "错误的数组偏移位移, 仅允许使用整数或整型变量");
                throw scope_critical_error();
            }

            if (child_type(0) == Token::Type::kIdentity) {
                result = Token(Token::Type::kIdentityArray, child(0)->token().content(), std::vector<std::string>({"1", child(1)->child(0)->token().content()}), current_->token().position());
            } else {
                add_error_messages(child(0)->token().position(), "错误的值 \"" + child(0)->token().content() + "\"");
                throw scope_critical_error();
            }
        }

        current_ = current_->parent();
        return result;
    }

    // 解析判别式
    Token analyse_condition(const int &pos) {
        current_ = child(pos);
        Token result;

        Token first = analyse_expression(0);
        Token op = analyse_comparison_op(1);
        Token second = analyse_expression(2);

        build_condition_ir(op);

        current_ = current_->parent();
        return result;
    }

    // 解析关系运算符
    Token analyse_comparison_op(const int &pos) {
        current_ = child(pos);
        Token result = child(0)->token();
        current_ = current_->parent();
        return result;
    }

    // 解析标识符
    Token analyse_identity(const int &pos) {
        Token result = child(pos)->token();
        if (result.type() == Token::Type::kIdentity) {
            return result;
        } else {
            add_error_messages(result.position(), "无效的标识符\"" + result.content() + "\", 仅允许变量");
            throw scope_critical_error();
        }
    }

    Token analyse_literal(const int &pos) {
        Token result = child(pos)->token();
        if (result.type() == Token::Type::kIdentity || result.type() == Token::Type::kIntegerLiteral || result.type() == Token::Type::kRealLiteral) {
            return result;
        } else {
            add_error_messages(result.position(), "无效的标识符\"" + result.content() + "\", 仅允许变量, 整数或实数类型");
            throw scope_critical_error();
        }
    }

    // 解析表达式
    Token analyse_expression(const int &pos) {
        current_ = child(pos);
        Token result;

        int offset = 0;
        result = analyse_term(offset++);
        while (offset < children_size()) {
            Token op = analyse_add_op(offset++);
            Token term = analyse_term(offset++);

            if (result.type() == Token::Type::kReal || term.type() == Token::Type::kReal) {
                result.set_type(Token::Type::kReal);
            }

            build_expression_ir(op);
        }

        current_ = current_->parent();
        return result;
    }

    // 解析算术式
    Token analyse_term(const int &pos) {
        current_ = child(pos);
        Token result;

        int offset = 0;
        result = analyse_factor(offset++);
        while (offset < children_size()) {
            Token op = analyse_mul_op(offset++);
            Token factor = analyse_factor(offset++);

            if (result.type() == Token::Type::kReal || factor.type() == Token::Type::kReal) {
                result.set_type(Token::Type::kReal);
            }

            build_term_ir(op);
        }

        current_ = current_->parent();
        return result;
    }

    // 解析元素
    Token analyse_factor(const int &pos) {
        current_ = child(pos);
        Token result, tmp;
        Token identity = child(0)->token();
        Symbol identity_symbol;
        std::string identity_string;

        switch (child_type(0)) {
            case Token::Type::kIntegerLiteral:
                result = Token(Token::Type::kInt, identity.position());
                ir_.add(PCode(PCode::Type::kPushInteger, child(0)->token().content(), ir_indent_));
                break;
            case Token::Type::kRealLiteral:
                result = Token(Token::Type::kReal, identity.position());
                ir_.add(PCode(PCode::Type::kPushReal, child(0)->token().content(), ir_indent_));
                break;
            case Token::Type::kIdentity:
                identity = child(0)->token();
                try {
                    identity_symbol = tree_.resolve(identity.content());
                } catch (const scope_not_found &e) {
                    add_error_messages(identity.position(), "未定义的标识符 \"" + identity.content() + "\"");
                    throw scope_critical_error();
                }
                if (!identity_symbol.is_assigned()) {
                    add_error_messages(identity.position(), "变量 \"" + identity.content() + "\" 尚未初始化而直接使用");
                    throw scope_critical_error();
                }

                if (identity_symbol.type() == Symbol::Type::kInt) {
                    result = Token(Token::Type::kInt, identity.position());
                    ir_.add(PCode(PCode::Type::kPushInteger, identity.content(), ir_indent_));
                } else if (identity_symbol.type() == Symbol::Type::kReal) {
                    result = Token(Token::Type::kReal, identity.position());
                    ir_.add(PCode(PCode::Type::kPushReal, identity.content(), ir_indent_));
                } else if (identity_symbol.type() == Symbol::Type::kIntArray || identity_symbol.type() == Symbol::Type::kRealArray) {
                    add_error_messages(identity.position(), "不能将整个数组 \"" + identity.content() + "\" 用于表达式计算过程中");
                    throw scope_critical_error();
                } else {
                    add_error_messages(identity.position(), "无法辨识的符号 \"" + identity.content() + "\"");
                    throw scope_critical_error();
                }
                break;
            case Token::Type::kIdentityArray:
                identity = analyse_identity_array(0);
                try {
                    identity_symbol = tree_.resolve(identity.content());
                } catch (const scope_not_found &e) {
                    add_error_messages(identity.position(), "未定义的标识符\"" + identity.content() + "\"");
                    throw scope_critical_error();
                }

                if (identity_symbol.type() == Symbol::Type::kInt || identity_symbol.type() == Symbol::Type::kReal) {
                    add_error_messages(identity.position(), "不能将数组运算符用于非数组变量 \"" + identity.content() + "\" 上");
                    throw scope_critical_error();
                } else if ((identity_symbol.type() == Symbol::Type::kIntArray && std::stoi(identity.extra().at(1)) >= identity_symbol.int_array().size()) ||
                           (identity_symbol.type() == Symbol::Type::kRealArray && std::stoi(identity.extra().at(1)) >= identity_symbol.real_array().size())) {
                    add_error_messages(identity.position(), "变量 \"" + identity.content() + "\" 的数组下标越界, 超过定义大小");
                    throw scope_critical_error();
                }

                if (identity_symbol.type() == Symbol::Type::kIntArray) {
                    result = Token(Token::Type::kInt, identity.position());
                    ir_.add(PCode(PCode::Type::kPushIntegerArray, identity.content(), identity.extra().at(1), ir_indent_));
                } else if (identity_symbol.type() == Symbol::Type::kRealArray) {
                    result = Token(Token::Type::kReal, identity.position());
                    ir_.add(PCode(PCode::Type::kPushRealArray, identity.content(), identity.extra().at(1), ir_indent_));
                } else {
                    add_error_messages(identity.position(), "系统内部错误: 无效返回 result 类型 at Token::Type::kIdentityArray");
                    throw scope_critical_error();
                }

                break;
            case Token::Type::kExpression:
                result = analyse_expression(0);
                break;
            case Token::Type::kFunctionCall:
                result = analyse_function_call(0);
                break;
            default:
                break;
        }

        current_ = current_->parent();
        return result;
    }

    Token analyse_add_op(const int &pos) {
        return child(pos)->child(0)->token();
    }

    Token analyse_mul_op(const int &pos) {
        return child(pos)->child(0)->token();
    }

    Token analyse_function_call(const int &pos) {
        current_ = child(pos);

        Symbol function_symbol;
        Token identity = child(0)->token();
        try {
            function_symbol = tree_.resolve(identity.content());
        } catch (const scope_not_found &e) {
            add_error_messages(identity.position(), "函数 \"" + identity.content() + "\" 未定义");
            throw scope_critical_error();
        }
        analyse_function_call_parameters(1, function_symbol);

        build_function_call_ir(identity);

        current_ = current_->parent();
        return Token(Symbol::convert_symbol_type(function_symbol.ret_type()), current_->token().position());
    }

    Token analyse_function_call_parameters(const int &pos, const Symbol &function_symbol) {
        current_ = child(pos);

        if (children_size() != function_symbol.args().size()) {
            add_error_messages(current_->token().position(), "函数 \"" + function_symbol.name() + "\" 需要传入 " + std::to_string(function_symbol.args().size()) + " 个参数, 实际只传入了 " + std::to_string(children_size()) + " 个参数");
            throw scope_critical_error();
        }
        for (int i = 0; i < children_size(); ++i) {
            Token expression = analyse_expression(i);
            if (Symbol::convert_token_type(expression.type()) != function_symbol.args().at((unsigned long)i).type()) {
                add_error_messages(expression.position(), "函数 \"" + function_symbol.name() + "\" 的第 " + std::to_string(i + 1) + " 个形参要求为 " + function_symbol.args().at((unsigned long)i).type_name() + " 类型, 实际传入参数类型为 " + expression.type_name());
                throw scope_critical_error();
            }
        }

        current_ = current_->parent();
        return current_->token();
    }

    void print_error_messages() const {
        for (int i = 0; i < error_messages_.size(); ++i) {
            std::cout << "[错误] 第 " << error_messages_[i].first.row() << " 行 第 " << error_messages_[i].first.col() << " 列: " << error_messages_[i].second << std::endl;
        }
    }

    void print_warning_messages() const {
        for (int i = 0; i < warning_messages_.size(); ++i) {
            std::cout << "[警告] 第 " << warning_messages_[i].first.row() << " 行 第 " << warning_messages_[i].first.col() << " 列: " << warning_messages_[i].second << std::endl;
        }
    }

    void print_ir() const {
        std::cout << ir_ << std::endl;
    }

private:
    AbstractSyntaxNode *root_;
    AbstractSyntaxNode *current_;
    ScopeTree tree_;
    IR ir_;
    int ir_indent_;
    std::vector<std::pair<Position, std::string> > error_messages_;
    std::vector<std::pair<Position, std::string> > warning_messages_;

    AbstractSyntaxNode *child(int offset) {
        return current_->children()[offset];
    }

    Token::Type child_type(int offset) {
        return child(offset)->token().type();
    }

    int children_size() {
        return (int)current_->children().size();
    }

    Token::Type current_token_type() {
        return current_->token().type();
    }

    void add_error_messages(const Position &position, const std::string &message) {
        error_messages_.push_back(std::pair<Position, std::string>(position, message));
    }

    void add_warning_messages(const Position &position, const std::string &message) {
        warning_messages_.push_back(std::pair<Position, std::string>(position, message));
    }

    void build_function_ir_start(const Token &identity) {
        ir_.add(PCode(PCode::Type::kStartFunc, identity.content(), ir_indent_));
    }

    void build_function_ir_args(const std::vector<std::pair<Token, Token> > &args) {
        for (int i = 0; i < (int)args.size(); ++i) {
            switch (args[i].first.type()) {
                case Token::Type::kInt:
                    ir_.add(PCode(PCode::Type::kArgInteger, args[i].second.content(), ir_indent_));
                    break;
                case Token::Type::kIntArray:
                    ir_.add(PCode(PCode::Type::kArgIntegerArray, args[i].second.content(), ir_indent_));
                    break;
                case Token::Type::kReal:
                    ir_.add(PCode(PCode::Type::kArgReal, args[i].second.content(), ir_indent_));
                    break;
                case Token::Type::kRealArray:
                    ir_.add(PCode(PCode::Type::kArgRealArray, args[i].second.content(), ir_indent_));
                    break;
                default:
                    break;
            }
        }
    }

    void build_function_ir_end(const Token &identity) {
        ir_.add(PCode(PCode::Type::kEndFunc, identity.content(), ir_indent_));
    }

    void build_return_statement_ir(const Token &literal) {
        ir_.add(PCode(PCode::Type::kReturn, "~", ir_indent_));
    }

    void build_declare_statement_ir(const Token &declare_keyword, const Token &identity) {
        switch (declare_keyword.type()) {
            case Token::Type::kInt:
                ir_.add(PCode(PCode::Type::kVarInteger, identity.content(), ir_indent_));
                break;
            case Token::Type::kIntArray:
                ir_.add(PCode(PCode::Type::kVarIntegerArray, identity.content(), declare_keyword.content(), ir_indent_));
                break;
            case Token::Type::kReal:
                ir_.add(PCode(PCode::Type::kVarReal, identity.content(), ir_indent_));
                break;
            case Token::Type::kRealArray:
                ir_.add(PCode(PCode::Type::kVarRealArray, identity.content(), declare_keyword.content(), ir_indent_));
                break;
            default:
                break;
        }
    }

    void build_assign_statement_ir(const Token &left_identity, const Token &right_expression) {
        if (left_identity.type() == Token::Type::kIdentityArray) {
            if (right_expression.type() == Token::Type::kInt) {
                ir_.add(PCode(PCode::Type::kPopIntegerArray, left_identity.content(), left_identity.extra().at(1), ir_indent_));
            } else if (right_expression.type() == Token::Type::kReal) {
                ir_.add(PCode(PCode::Type::kPopRealArray, left_identity.content(), left_identity.extra().at(1), ir_indent_));
            } else {
                throw std::invalid_argument("build_assign_statement_ir 1 failed, not expected.");
            }
        } else if (left_identity.type() == Token::Type::kIdentity) {
            if (right_expression.type() == Token::Type::kInt) {
                ir_.add(PCode(PCode::Type::kPopInteger, left_identity.content(), ir_indent_));
            } else if (right_expression.type() == Token::Type::kReal) {
                ir_.add(PCode(PCode::Type::kPopReal, left_identity.content(), ir_indent_));
            } else {
                throw std::invalid_argument("build_assign_statement_ir 2 failed, not expected.");
            }
        } else {
            throw std::invalid_argument("build_assign_statement_ir 3 failed, not expected.");
        }
    }

    void build_expression_ir(const Token &op) {
        if (op.type() == Token::Type::kPlus) {
            ir_.add(PCode(PCode::Type::kAdd, ir_indent_));
        } else {
            ir_.add(PCode(PCode::Type::kSub, ir_indent_));
        }
    }

    void build_term_ir(const Token &op) {
        if (op.type() == Token::Type::kTimes) {
            ir_.add(PCode(PCode::Type::kMul, ir_indent_));
        } else {
            ir_.add(PCode(PCode::Type::kDiv, ir_indent_));
        }
    }

    void build_function_call_ir(const Token &identity) {
        ir_.add(PCode(PCode::Type::kCall, identity.content(), ir_indent_));
    }

    void build_write_statement_ir() {
        ir_.add(PCode(PCode::Type::kPrint, ir_indent_));
    }

    void build_read_statement_ir(const Token &token, const Symbol &symbol) {
        if (token.type() == Token::Type::kIdentity) {
            if (symbol.type() == Symbol::Type::kInt) {
                ir_.add(PCode(PCode::Type::kReadInt, token.content(), ir_indent_));
            } else if (symbol.type() == Symbol::Type::kReal) {
                ir_.add(PCode(PCode::Type::kReadReal, token.content(), ir_indent_));
            } else {
                throw std::invalid_argument("build_read_statement_ir 1 failed, not expected.");
            }
        } else if (token.type() == Token::Type::kIdentityArray) {
            if (symbol.type() == Symbol::Type::kIntArray) {
                ir_.add(PCode(PCode::Type::kReadIntArray, token.content(), token.extra().at(1), ir_indent_));
            } else if (symbol.type() == Symbol::Type::kRealArray) {
                ir_.add(PCode(PCode::Type::kReadRealArray, token.content(), token.extra().at(1), ir_indent_));
            } else {
                throw std::invalid_argument("build_read_statement_ir 2 failed, not expected.");
            }
        } else {
            throw std::invalid_argument("build_read_statement_ir 3 failed, not expected.");
        }
    }

    void build_condition_ir(const Token &op) {
        switch (op.type()) {
            case Token::Type::kLT:
                ir_.add(PCode(PCode::Type::kCompareLessThan, ir_indent_));
                break;
            case Token::Type::kLTE:
                ir_.add(PCode(PCode::Type::kCompareLessEqual, ir_indent_));
                break;
            case Token::Type::kGT:
                ir_.add(PCode(PCode::Type::kCompareGreaterThan, ir_indent_));
                break;
            case Token::Type::kGTE:
                ir_.add(PCode(PCode::Type::kCompareGreaterEqual, ir_indent_));
                break;
            case Token::Type::kEqual:
                ir_.add(PCode(PCode::Type::kCompareEqual, ir_indent_));
                break;
            case Token::Type::kNotEqual:
                ir_.add(PCode(PCode::Type::kCompareNotEqual, ir_indent_));
                break;
            default:
                throw std::invalid_argument("build_condition_ir failed, not expected.");
        }
    }

    void build_while_statement_ir_begin(const std::string &signature) {
        ir_.add(PCode(PCode::Type::kLabel, "_beg_while" + signature, ir_indent_));
    }

    void build_while_statement_ir_jz(const std::string &signature) {
        ir_.add(PCode(PCode::Type::kJumpZero, "_end_while" + signature, ir_indent_));
    }

    void build_while_statement_ir_end(const std::string &signature) {
        ir_.add(PCode(PCode::Type::kJump, "_begin_while" + signature, ir_indent_));
        ir_.add(PCode(PCode::Type::kLabel, "_end_while" + signature, ir_indent_));
    }

    void build_if_statement_ir_begin(const std::string &signature) {
        ir_.add(PCode(PCode::Type::kLabel, "_beg_if" + signature, ir_indent_));
    }

    void build_if_statement_ir_jz(const std::string &signature) {
        ir_.add(PCode(PCode::Type::kJumpZero, "_else" + signature, ir_indent_));
    }

    void build_if_statement_ir_else(const std::string &signature) {
        ir_.add(PCode(PCode::Type::kJump, "_end_if" + signature, ir_indent_));
        ir_.add(PCode(PCode::Type::kLabel, "_else" + signature, ir_indent_));
    }

    void build_if_statement_ir_end(const std::string &signature) {
        ir_.add(PCode(PCode::Type::kLabel, "_end_if" + signature, ir_indent_));
    }
};

#endif //CMM_SEMANTIC_H


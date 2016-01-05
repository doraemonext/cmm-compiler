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
                    parameters_symbol.push_back(Symbol(second.content(), 0, false));
                    break;
                case Token::Type::kIntArray:
                    parameters_symbol.push_back(Symbol(second.content(), std::vector<int>(), false));
                    break;
                case Token::Type::kReal:
                    parameters_symbol.push_back(Symbol(second.content(), 0.0, false));
                    break;
                case Token::Type::kRealArray:
                    parameters_symbol.push_back(Symbol(second.content(), std::vector<double>(), false));
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
                break;
            case Token::Type::kWhileStatement:
                break;
            case Token::Type::kReadStatement:
                break;
            case Token::Type::kWriteStatement:
                break;
            case Token::Type::kAssignStatement:
                break;
            case Token::Type::kDeclareStatement:
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

    // 解析 return 语句
    Token analyse_return_statement(const int &pos, const Symbol &function_symbol) {
        current_ = child(pos);

        Token result = analyse_literal(0);
        if (result.type() == Token::Type::kIdentity) {
            try {
                const Symbol &identity = tree_.resolve(result.content());
                if (identity.type() != function_symbol.ret_type()) {
                    add_error_messages(result.position(), "return 语句返回的 \"" + identity.name() + "\" 与函数 \"" + function_symbol.name() + "\" 的返回类型不一致");
                    throw scope_critical_error();
                }
            } catch (const scope_not_found &e) {
                add_error_messages(result.position(), "未定义的变量 \"" + result.content() + "\" 不能用于 return 语句");
                throw scope_critical_error();
            }
        } else if (result.type() == Token::Type::kIntegerLiteral) {
            if (function_symbol.ret_type() != Symbol::Type::kInt) {
                add_error_messages(result.position(), "return 语句返回的 \"" + result.content() + "\" 与函数 \"" + function_symbol.name() + "\" 的返回类型不一致");
                throw scope_critical_error();
            }
        } else if (result.type() == Token::Type::kRealLiteral) {
            if (function_symbol.ret_type() != Symbol::Type::kReal) {
                add_error_messages(result.position(), "return 语句返回的 \"" + result.content() + "\" 与函数 \"" + function_symbol.name() + "\" 的返回类型不一致");
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
                add_error_messages(child(0)->token().position(), "错误的函数定义类型 \"" + std::string(Token::token_type_name(child_type(0))) + "\"");
                throw scope_critical_error();
            }
        } else {
            if (child_type(0) == Token::Type::kInt) {
                result = Token(Token::Type::kIntArray, child(1)->child(0)->token().content(), current_->token().position());
            } else if (child_type(0) == Token::Type::kReal) {
                result = Token(Token::Type::kRealArray, child(1)->child(0)->token().content(), current_->token().position());
            } else {
                add_error_messages(child(0)->token().position(), "错误的函数定义类型 \"" + std::string(Token::token_type_name(child_type(0))) + "\"");
                throw scope_critical_error();
            }
        }

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
        ir_.add(PCode(PCode::Type::kReturn, literal.content(), ir_indent_));
    }
};

#endif //CMM_SEMANTIC_H

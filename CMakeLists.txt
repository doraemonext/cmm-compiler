cmake_minimum_required(VERSION 3.3.2)
project(cmm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp include/token.h include/utils.h include/lexer.h include/exceptions.h token.cpp lexer.cpp include/parser.h include/symbol.h symbol.cpp include/scope.h scope.cpp include/ast.h parser.cpp include/semantic.h include/ir.h include/simulator.h)
add_executable(cmm ${SOURCE_FILES})
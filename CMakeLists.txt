cmake_minimum_required(VERSION 3.4)
project(cmm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp include/token.h include/utils.h include/lexer.h include/exceptions.h)
add_executable(cmm ${SOURCE_FILES})
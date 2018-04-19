cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(
    -Wall -Wpedantic -Wextra -Wunused -Wfloat-equal -Wpointer-arith -Wwrite-strings

#    # Disabled warnings:
#     -Wno-missing-braces # false-positive on std::array initialization.

     -Werror
)

add_library(
    function_parser

    FunctionParser.cpp
    Tokenizer.cpp
    Lexer.cpp
)

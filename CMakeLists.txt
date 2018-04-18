cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(function_parser)

add_executable(function_parser
    FunctionParser.cpp
    Tokenizer.cpp
    main.cpp
)

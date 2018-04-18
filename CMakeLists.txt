cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(function_parser)

add_subdirectory(./src)
add_subdirectory(./test)
add_subdirectory(./third-party/gtest/)

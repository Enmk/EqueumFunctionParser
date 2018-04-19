
add_executable(test_function_parser

    main.cpp
    test_Tokenizer.cpp
    test_Lexer.cpp
    test_FunctionParser.cpp
    test_FunctionRegistry.cpp

    Utility.cpp
)

target_include_directories(test_function_parser
    PRIVATE
    ../third-party/gtest/googletest/include
    ../src
)

target_link_libraries(test_function_parser
    PRIVATE
    gtest
    function_parser
)

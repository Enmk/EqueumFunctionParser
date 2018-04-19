/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "Lexer.h"

#include <gtest/gtest.h>

#include <boost/preprocessor/stringize.hpp>

#include <ostream>
#include <unordered_map>
#include <vector>

inline std::ostream& operator<<(std::ostream& ostr, LexemeType lexType)
{
#define TYPE_STRING(T) {T, BOOST_PP_STRINGIZE(T)}
    static const std::unordered_map<size_t, const char*> LexTypeNames =
    {
        TYPE_STRING(LEX_NUMBER_LITERAL),
        TYPE_STRING(LEX_STRING_LITERAL),
        TYPE_STRING(LEX_NAME),
        TYPE_STRING(LEX_OPERATOR),
        TYPE_STRING(LEX_PUNCTUATION),
        TYPE_STRING(LEX_LEFT_PARENTHESIS),
        TYPE_STRING(LEX_RIGHT_PARENTHESIS),
        TYPE_STRING(LEX_END_OF_INPUT)
    };
    return ostr << LexTypeNames.at(lexType);
}

inline std::ostream& operator<<(std::ostream& ostr, const Lexeme& lex)
{
    return ostr << "Lexeme{\"" << lex.value << "\", " << lex.type << "}";
}

inline bool operator==(const Lexeme& left, const Lexeme& right)
{
    return left.type == right.type && left.value == right.value;
}

static const Lexeme EofLexeme{std::string(), LEX_END_OF_INPUT};

struct LexerTestCase
{
    const char* input;
    std::vector<Lexeme> expectedLexemes;
};

inline std::ostream& operator<<(std::ostream& ostr, const LexerTestCase& testCase)
{
    ostr << "LexerTestCase{\n\t\"" << testCase.input << "\"";
    for (const auto& l : testCase.expectedLexemes)
    {
        ostr << ",\n\t" << l;
    }

    return ostr << "\n}";
}

#define ONE_LEXEME_TEST_CASE(input, type)     {input, {Lexeme{input, type}}}

const LexerTestCase LexSimpleTestCases[] =
{
    ONE_LEXEME_TEST_CASE("abc123", LEX_NAME),
    ONE_LEXEME_TEST_CASE("123.456", LEX_NUMBER_LITERAL),
    ONE_LEXEME_TEST_CASE(R"(" some \"fancy string \\")", LEX_STRING_LITERAL),
    ONE_LEXEME_TEST_CASE("(", LEX_LEFT_PARENTHESIS),
    ONE_LEXEME_TEST_CASE(")", LEX_RIGHT_PARENTHESIS),
    ONE_LEXEME_TEST_CASE("+", LEX_OPERATOR),
    ONE_LEXEME_TEST_CASE("=", LEX_OPERATOR),
    ONE_LEXEME_TEST_CASE("", LEX_END_OF_INPUT),
};

const LexerTestCase LexCompoundTestCases[] =
{
//    {
//        "abc123(a=1)",
//        {
//            Lexeme{"abc123", LEX_NAME},
//            Lexeme{"(", LEX_LEFT_PARENTHESIS},
//            Lexeme{"a", LEX_NAME},
//            Lexeme{"=", LEX_OPERATOR},
//            Lexeme{"1", LEX_NUMBER_LITERAL},
//            Lexeme{")", LEX_RIGHT_PARENTHESIS},
//        }
//    },
//    {
//        // same case as above, but with whitespaces.
//        " abc123 ( a = 1 ) ",
//        {
//            Lexeme{"abc123", LEX_NAME},
//            Lexeme{"(", LEX_LEFT_PARENTHESIS},
//            Lexeme{"a", LEX_NAME},
//            Lexeme{"=", LEX_OPERATOR},
//            Lexeme{"1", LEX_NUMBER_LITERAL},
//            Lexeme{")", LEX_RIGHT_PARENTHESIS},
//        }
//    },
    {
        R"( abc123(first=4.56, second="foo") )",
        {
            Lexeme{"abc123", LEX_NAME},
            Lexeme{"(", LEX_LEFT_PARENTHESIS},
            Lexeme{"first", LEX_NAME},
            Lexeme{"=", LEX_OPERATOR},
            Lexeme{"4.56", LEX_NUMBER_LITERAL},
            Lexeme{",", LEX_PUNCTUATION},
            Lexeme{"second", LEX_NAME},
            Lexeme{"=", LEX_OPERATOR},
            Lexeme{R"("foo")", LEX_STRING_LITERAL},
            Lexeme{")", LEX_RIGHT_PARENTHESIS},
        }
    }
};

class LexerTest : public ::testing::TestWithParam<LexerTestCase>
{};

TEST_P(LexerTest, getNextLexeme)
{
    const LexerTestCase& testCase = GetParam();
    Lexer lexer(testCase.input);

    for (const auto& expectedLexeme : testCase.expectedLexemes)
    {
        Lexeme lexeme = lexer.getNextLexeme();
        EXPECT_EQ(expectedLexeme, lexeme);
    }
}

//INSTANTIATE_TEST_CASE_P(
//        Simple, LexerTest,
//        ::testing::ValuesIn(LexSimpleTestCases),
//);

INSTANTIATE_TEST_CASE_P(
        Compound, LexerTest,
        ::testing::ValuesIn(LexCompoundTestCases),
);

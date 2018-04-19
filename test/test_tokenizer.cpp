/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "Tokenizer.h"

#include <gtest/gtest.h>

#include <boost/preprocessor/stringize.hpp>

#include <ostream>
#include <unordered_map>
#include <vector>

inline std::ostream& operator<<(std::ostream& ostr, TokenType tokenType)
{
#define TYPE_STRING(T) {T, BOOST_PP_STRINGIZE(T)}
    static const std::unordered_map<size_t, const char*> TokenTypeNames =
    {
        TYPE_STRING(TOKEN_WHITESPACE),
        TYPE_STRING(TOKEN_LPAR),
        TYPE_STRING(TOKEN_RPAR),
        TYPE_STRING(TOKEN_OP),
        TYPE_STRING(TOKEN_PUNCT),
        TYPE_STRING(TOKEN_STRING),
        TYPE_STRING(TOKEN_QUOTED_STRING),
        TYPE_STRING(TOKEN_NUMBER),
        TYPE_STRING(TOKEN_END_OF_INPUT),
    };
    return ostr << TokenTypeNames.at(tokenType);
}

inline std::ostream& operator<<(std::ostream& ostr, const Token& token)
{
    return ostr << "Token{\"" << token.value << "\", " << token.type << "}";
}

inline bool operator==(const Token& left, const Token& right)
{
    return left.type == right.type && left.value == right.value;
}

static const Token EofToken{boost::string_view(), TOKEN_END_OF_INPUT};

struct TokenTestCase
{
    const char* input;
    const std::vector<Token> expectedTokens;
};

inline std::ostream& operator<<(std::ostream& ostr, const TokenTestCase& testCase)
{
    ostr << "TokenTestCase{ \"" << testCase.input << "\"";
    for (const auto& t : testCase.expectedTokens)
    {
        ostr << ",\n\t" << t;
    }

    return ostr << "}";
}

class TokenTest : public ::testing::TestWithParam<TokenTestCase>
{};

TEST_P(TokenTest, getNextToken)
{
    const TokenTestCase& testCase = GetParam();

    Tokenizer tokenizer(testCase.input);
    for (const auto& expectedToken : testCase.expectedTokens)
    {
        const Token actualToken = tokenizer.getNextToken();

        ASSERT_EQ(expectedToken, actualToken);
    }

    ASSERT_EQ(EofToken, tokenizer.getNextToken());
}

#define ONE_TOKEN_TEST_CASE(input, type)     {input, {Token{input, type}}}

const TokenTestCase TokenOneTypeTestCases[] =
{
    ONE_TOKEN_TEST_CASE(" \v\t\n\r ", TOKEN_WHITESPACE),
    ONE_TOKEN_TEST_CASE("123", TOKEN_NUMBER),
    ONE_TOKEN_TEST_CASE("0", TOKEN_NUMBER),
    ONE_TOKEN_TEST_CASE("abcd", TOKEN_STRING),
    ONE_TOKEN_TEST_CASE("+", TOKEN_OP),
    ONE_TOKEN_TEST_CASE("-", TOKEN_OP),
    ONE_TOKEN_TEST_CASE("*", TOKEN_OP),
    ONE_TOKEN_TEST_CASE("/", TOKEN_OP),
    ONE_TOKEN_TEST_CASE("=", TOKEN_OP),
    ONE_TOKEN_TEST_CASE("(", TOKEN_LPAR),
    ONE_TOKEN_TEST_CASE(")", TOKEN_RPAR),
    ONE_TOKEN_TEST_CASE(".", TOKEN_PUNCT),
    ONE_TOKEN_TEST_CASE(",", TOKEN_PUNCT),
    ONE_TOKEN_TEST_CASE(":", TOKEN_PUNCT),
    ONE_TOKEN_TEST_CASE(";", TOKEN_PUNCT),
    ONE_TOKEN_TEST_CASE(R"("")", TOKEN_QUOTED_STRING),
    ONE_TOKEN_TEST_CASE(R"("abc")", TOKEN_QUOTED_STRING),
    ONE_TOKEN_TEST_CASE(R"(" ")", TOKEN_QUOTED_STRING),
    ONE_TOKEN_TEST_CASE(R"("\"")", TOKEN_QUOTED_STRING),
};

const TokenTestCase TokenSplitSequenceTestCases[] =
{
    {
        "((",
        {
            Token{"(", TOKEN_LPAR},
            Token{"(", TOKEN_LPAR},
        }
    },
    {
        "))",
        {
            Token{")", TOKEN_RPAR},
            Token{")", TOKEN_RPAR},
        }
    },
    {
        "+-*/=",
        {
            Token{"+", TOKEN_OP},
            Token{"-", TOKEN_OP},
            Token{"*", TOKEN_OP},
            Token{"/", TOKEN_OP},
            Token{"=", TOKEN_OP},
        }
    },
};

const TokenTestCase TokenCompoundTestCases[] =
{
    {
        "a1()",
        {
            Token{"a", TOKEN_STRING},
            Token{"1", TOKEN_NUMBER},
            Token{"(", TOKEN_LPAR},
            Token{")", TOKEN_RPAR},
        }
    },
    {
        "(1( )2)",
        {
            Token{"(", TOKEN_LPAR},
            Token{"1", TOKEN_NUMBER},
            Token{"(", TOKEN_LPAR},
            Token{" ", TOKEN_WHITESPACE},
            Token{")", TOKEN_RPAR},
            Token{"2", TOKEN_NUMBER},
            Token{")", TOKEN_RPAR},
        }
    },
    {
        "function_name(arg=1)",
        {
            Token{"function_name", TOKEN_STRING},
            Token{"(", TOKEN_LPAR},
            Token{"arg", TOKEN_STRING},
            Token{"=", TOKEN_OP},
            Token{"1", TOKEN_NUMBER},
            Token{")", TOKEN_RPAR},
        }
    },
    {
        "function_name123test(arg=1)",
        {
            Token{"function_name", TOKEN_STRING},
            Token{"123", TOKEN_NUMBER},
            Token{"test", TOKEN_STRING},
            Token{"(", TOKEN_LPAR},
            Token{"arg", TOKEN_STRING},
            Token{"=", TOKEN_OP},
            Token{"1", TOKEN_NUMBER},
            Token{")", TOKEN_RPAR},
        }
    }
};

INSTANTIATE_TEST_CASE_P(
        OneType, TokenTest,
        ::testing::ValuesIn(TokenOneTypeTestCases),
);

INSTANTIATE_TEST_CASE_P(
        SplitSequence, TokenTest,
        ::testing::ValuesIn(TokenSplitSequenceTestCases),
);

INSTANTIATE_TEST_CASE_P(
        CompoundType, TokenTest,
        ::testing::ValuesIn(TokenCompoundTestCases),
);

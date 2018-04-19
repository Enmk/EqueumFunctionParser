/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "FunctionParser.h"

#include "Utility.h"

#include <gtest/gtest.h>

#include <ostream>
#include <unordered_map>
#include <vector>

namespace
{
const boost::optional<std::string> UnsetOptionalString;
} // namespace

struct FunctionParserSpecTestCase
{
    const char* input;
    const FunctionSpec spec;
};

inline std::ostream& operator<<(std::ostream& ostr, const FunctionParserSpecTestCase& testCase)
{
    ostr << "FunctionParserSpecTestCase{\n\t\"" << testCase.input << "\""
            << ",\n\t" << testCase.spec.name << "(";

    for (const auto& p : testCase.spec.parameters)
    {
        ostr << " " << p;

        if (&p != &testCase.spec.parameters.back())
        {
            ostr << ", ";
        }
    }

    return ostr << ")\n}";
}

const FunctionParserSpecTestCase SpecTestCases[] =
{
    {
        "a()",
        {
            "a",
            {}
        }
    },
    {
        "abcdef123()",
        {
            "abcdef123",
            {}
        }
    },
    {
        "function(a,b,c=1)",
        {
            "function",
            {
                FunctionSpecParameter{"a", UnsetOptionalString},
                FunctionSpecParameter{"b", UnsetOptionalString},
                FunctionSpecParameter{"c", std::string("1")}
            }
        }
    },
    {
        R"(function(a, b , c = 1, d = "foobar") )",
        {
            "function",
            {
                FunctionSpecParameter{"a", UnsetOptionalString},
                FunctionSpecParameter{"b", UnsetOptionalString},
                FunctionSpecParameter{"c", std::string("1")},
                FunctionSpecParameter{"d", std::string(R"("foobar")")}
            }
        }
    }
};

class FunctionParserSpecTest : public ::testing::TestWithParam<FunctionParserSpecTestCase>
{};

TEST_P(FunctionParserSpecTest, parseFunctionSpec)
{
    const FunctionParserSpecTestCase& testCase = GetParam();
    FunctionSpec spec = parseFunctionSpec(testCase.input);

    EXPECT_EQ(testCase.spec.name, spec.name);
    EXPECT_EQ(testCase.spec.parameters.size(), spec.parameters.size());
    for (int i = 0; i < testCase.spec.parameters.size(); ++i)
    {
        EXPECT_EQ(testCase.spec.parameters[i], spec.parameters[i]);
    }
}

INSTANTIATE_TEST_CASE_P(
        Simple, FunctionParserSpecTest,
        ::testing::ValuesIn(SpecTestCases),
);

struct FunctionParserCallTestCase
{
    const char* input;
    const FunctionCall call;
};

inline std::ostream& operator<<(std::ostream& ostr, const FunctionParserCallTestCase& testCase)
{
    return ostr << "ParseFunctionCallTestCase{\n\t\"" << testCase.input << "\""
            << ",\n\t" << testCase.call << "\n}";
}

const FunctionParserCallTestCase CallTestCases[] =
{
    {
        "a()",
        {
            "a",
            {}
        }
    },
    {
        "abcdef123()",
        {
            "abcdef123",
            {}
        }
    },
    {
        "function(1,2,c=3)",
        {
            "function",
            {
                FunctionCallParameter{UnsetOptionalString, "1"},
                FunctionCallParameter{UnsetOptionalString, "2"},
                FunctionCallParameter{std::string("c"), "3"}
            }
        }
    },
    {
        R"(function("a", 2 , c = 3, d = "foobar") )",
        {
            "function",
            {
                FunctionCallParameter{UnsetOptionalString, R"("a")"},
                FunctionCallParameter{UnsetOptionalString, "2"},
                FunctionCallParameter{std::string("c"), "3"},
                FunctionCallParameter{std::string("d"), R"("foobar")"},
            }
        }
    }
};

class FunctionParserCallTest : public ::testing::TestWithParam<FunctionParserCallTestCase>
{};

TEST_P(FunctionParserCallTest, parseFunctionCall)
{
    const FunctionParserCallTestCase& testCase = GetParam();
    FunctionCall call = parseFunctionCall(testCase.input);

    EXPECT_EQ(testCase.call.name, call.name);
    EXPECT_EQ(testCase.call.parameters.size(), call.parameters.size());
    for (int i = 0; i < testCase.call.parameters.size(); ++i)
    {
        EXPECT_EQ(testCase.call.parameters[i], call.parameters[i]);
    }
}


INSTANTIATE_TEST_CASE_P(
        Simple, FunctionParserCallTest,
        ::testing::ValuesIn(CallTestCases),
);

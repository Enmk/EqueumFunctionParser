/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "FunctionParser.h"

#include <gtest/gtest.h>


#include <ostream>
#include <unordered_map>
#include <vector>

namespace
{
const boost::optional<std::string> UnsetOptionalString;
} // namespace

bool operator==(const FunctionSpecParameter& left, const FunctionSpecParameter& right)
{
    return left.name == right.name && left.value == right.value;
}

struct FunctionParserSpecTestCase
{
    const char* input;
    const FunctionSpec spec;
};

inline std::ostream& operator<<(std::ostream& ostr, const FunctionSpecParameter& spec)
{
    ostr << spec.name;
    if (spec.value)
    {
        ostr << "=" << *spec.value;
    }

    return ostr;
}

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

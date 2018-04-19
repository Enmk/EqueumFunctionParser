/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "FunctionRegistry.h"
#include "FunctionParser.h"

#include "Utility.h"

#include <gtest/gtest.h>

#include <boost/optional.hpp>

#include <ostream>
#include <unordered_map>
#include <vector>

namespace
{
const boost::optional<std::string> UnsetOptionalString;

struct FunctionRegistryTestCase
{
    const char* input;
    const char* call;
    const FunctionCall updatedCall;
};

std::ostream& operator<<(std::ostream& ostr, const FunctionRegistryTestCase& testCase)
{
    return ostr << "FunctionRegistryTestCase{\n\t" << testCase.input
             << ",\n\t" << "original: " << testCase.call
             << ",\n\t" << "updated: " << testCase.updatedCall
             << "}";
}

const FunctionRegistryTestCase FunctionRegistryTestCases[] =
{
    {
        "funcky_function(a, b=24, c=56, d=78)",
        // Original:
       "funcky_function(1, d=10)",
        // Updated:
        FunctionCall
        {
            "funcky_function",
            {
                FunctionCallParameter{std::string("a"), "1"},
                FunctionCallParameter{std::string("b"), "24"},
                FunctionCallParameter{std::string("c"), "56"},
                FunctionCallParameter{std::string("d"), "10"}
            }
        }
    }
};

} // namespace

class FunctionRegistryTest : public ::testing::TestWithParam<FunctionRegistryTestCase>
{};

TEST_P(FunctionRegistryTest, BasicTest)
{
    const auto& testCase = GetParam();

    FunctionRegistry registry;
    const auto name = registry.addFunction(testCase.input);
    const FunctionCall call = parseFunctionCall(testCase.call);
    const FunctionCall updated = registry.updateFunctionCall(call);

    ASSERT_EQ(testCase.updatedCall, updated);
}

INSTANTIATE_TEST_CASE_P(
        Simple, FunctionRegistryTest,
        ::testing::ValuesIn(FunctionRegistryTestCases),
);

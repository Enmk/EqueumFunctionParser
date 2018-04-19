/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/
#ifndef EQUEUM_FUNCTION_PARSER_FUNCTION_PARSER_H_INCLUDED
#define EQUEUM_FUNCTION_PARSER_FUNCTION_PARSER_H_INCLUDED

#include <boost/optional.hpp>

#include <cstddef>
#include <exception>
#include <string>
#include <vector>

struct FunctionSpecParameter
{
    std::string name;
    boost::optional<std::string> value;
};

struct FunctionSpec
{
    std::string name;
    std::vector<FunctionSpecParameter> parameters;
};

struct FunctionCallParameter
{
    boost::optional<std::string> name;
    std::string value;
};

struct FunctionCall
{
    std::string name;
    std::vector<FunctionCallParameter> parameters;
};

FunctionSpec parseFunctionSpec(const std::string& input);
FunctionCall parseFunctionCall(const std::string& input);

#endif // EQUEUM_FUNCTION_PARSER_FUNCTION_PARSER_H_INCLUDED

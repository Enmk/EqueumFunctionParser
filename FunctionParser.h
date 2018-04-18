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

struct FunctionParameterSpec
{
    std::string name;
    boost::optional<std::string> defaultValue;
};

struct FunctionSpec
{
    std::string name;
    std::vector<FunctionParameterSpec> parametersSpec;
};

class ParserException : public std::exception
{
public:
    ParserException(
            std::string message,
            std::string input,
            size_t position);

    const char* what() const noexcept override;
    std::string getInput() const;
    size_t getPosition() const;

private:
    const std::string message;
    const std::string input;
    const size_t position;
};

/** Checks if given string is a valid name.
 * Name rules are the same for functions and arguments:
 * [_a-zA-Z][_a-zA-Z0-9]*
 *
 * @param name - value to verify.
 * @return true if name is valid name according to language rules.
 */
bool isValidName(const std::string& name) noexcept;

/** Parse function spec from given input.
 *
 * Parsing is performed by skipping any leading whitespace first.
 * Input is modified to contain only symbols past function spec.
 *
 * @param input - input string containing function spec.
 * @return FunctionSpec object.
 * @throw exception if input has no function spec.
 */
FunctionSpec parseFunctionSpec(std::string& input);

#endif // EQUEUM_FUNCTION_PARSER_FUNCTION_PARSER_H_INCLUDED

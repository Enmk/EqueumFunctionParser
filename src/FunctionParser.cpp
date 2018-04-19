/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "FunctionParser.h"
#include "Tokenizer.h"

#include <regex>

namespace
{
const char* NameRegexPattern = "[_a-zA-Z][_a-zA-Z0-9]*";
} // namespace

ParserException::ParserException(
        std::string message,
        std::string input,
        size_t position)
    : message(std::move(message)),
      input(std::move(input)),
      position(position)
{
}

const char* ParserException::what() const noexcept
{
    return message.c_str();
}

std::string ParserException::getInput() const
{
    return input;
}

size_t ParserException::getPosition() const
{
    return position;
}

bool isValidName(const std::string& name) noexcept
{
    static const std::regex NameRegex(NameRegexPattern);
    return std::regex_match(name, NameRegex);
}

FunctionSpec parseFunctionSpec(std::string& /*input*/)
{
    return FunctionSpec();
}

/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "FunctionParser.h"
#include "Lexer.h"

#include <regex>

namespace
{

void validateFunctionSpec(const FunctionSpec& /*spec*/)
{
}

void validateFunctionCall(const FunctionCall& /*spec*/)
{
}

} // namespace


FunctionSpec parseFunctionSpec(const std::string& input)
{
    FunctionSpec result;
    Lexer lexer(input);

    Lexeme lex = lexer.getNextLexeme();
    assert(lex.type == LEX_NAME);
    result.name = lex.value;

    assert(lexer.getNextLexeme().type == LEX_LEFT_PARENTHESIS);
    // parsing arguments
    while (true)
    {
        lex = lexer.getNextLexeme();
        if (lex.type == LEX_RIGHT_PARENTHESIS && lex.value == ")")
        {
            // do not expect nested parenthesis here.
            break;
        }
        if (lex.type == LEX_PUNCTUATION && lex.value == ",")
        {
            assert(!result.parameters.empty() && "Stray comma");

            // skip to the next argument.
            continue;
        }

        result.parameters.push_back(FunctionSpecParameter{});
        FunctionSpecParameter& param = result.parameters.back();

        assert(lex.type == LEX_NAME);
        param.name = lex.value;

        lex = lexer.getNextLexeme();
        if (lex.type == LEX_OPERATOR && lex.value == "=")
        {
            // next is default value

            lex = lexer.getNextLexeme();
            assert(lex.type == LEX_NUMBER_LITERAL || lex.type == LEX_STRING_LITERAL);
            param.value = lex.value;
        }
    }

    validateFunctionSpec(result);

    return result;
}

FunctionCall parseFunctionCall(const std::string& input)
{
    FunctionCall result;
    Lexer lexer(input);

    Lexeme lex = lexer.getNextLexeme();
    assert(lex.type == LEX_NAME);
    result.name = lex.value;

    assert(lexer.getNextLexeme().type == LEX_LEFT_PARENTHESIS);
    // parsing arguments
    while (true)
    {
        lex = lexer.getNextLexeme();
        if (lex.type == LEX_RIGHT_PARENTHESIS && lex.value == ")")
        {
            // do not expect nested parenthesis here.
            break;
        }
        if (lex.type == LEX_PUNCTUATION && lex.value == ",")
        {
            assert(!result.parameters.empty() && "Stray comma");

            // skip to the next argument.
            continue;
        }

        result.parameters.push_back(FunctionCallParameter{});
        FunctionCallParameter& param = result.parameters.back();

        if (lex.type == LEX_NAME)
        {
            param.name = lex.value;
            lex = lexer.getNextLexeme();
            assert(lex.type == LEX_OPERATOR && lex.value == "=");
            lex = lexer.getNextLexeme();
        }
        assert(lex.type == LEX_NUMBER_LITERAL || lex.type == LEX_STRING_LITERAL);
        param.value = lex.value;
    }

    validateFunctionCall(result);

    return result;
}

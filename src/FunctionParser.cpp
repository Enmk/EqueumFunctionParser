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
            // skip to the next argument.
            continue;
        }

        result.parameters.push_back(FunctionSpecParameter{});
        FunctionSpecParameter& paramSpec = result.parameters.back();

        assert(lex.type == LEX_NAME);
        paramSpec.name = lex.value;

        lex = lexer.getNextLexeme();
        if (lex.type == LEX_OPERATOR && lex.value == "=")
        {
            // next is default value

            lex = lexer.getNextLexeme();
            assert(lex.type == LEX_NUMBER_LITERAL || lex.type == LEX_STRING_LITERAL);
            paramSpec.value = lex.value;
        }
    }

    validateFunctionSpec(result);

    return result;
}

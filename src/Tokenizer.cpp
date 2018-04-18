/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "Tokenizer.h"

namespace
{
TokenType getCharacterTokenType(char c)
{
    switch(c)
    {
        case '(':
            return TOKEN_LPAR;
        case ')':
            return TOKEN_RPAR;
        case '=':
        case '+':
        case '-':
        case '/':
        case '*':
            return TOKEN_OP;
    }
    if (isspace(c))
    {
        return TOKEN_WHITESPACE;
    }
    else if (isnumber(c) || c == '.')
    {
        return TOKEN_NUMBER_LITERAL;
    }
    else
    {
        return TOKEN_STRING_LITERAL;
    }
}

size_t findLengthOfStringLiteral(const boost::string_view& str)
{
    bool is_escaped = false;
    bool is_quoted = false;

    for (size_t i = 0; i < str.length(); ++i)
    {
        const char c = str[i];
        if (c == '\\')
        {
            is_escaped = !is_escaped;
        }
        else if (c == '"' && !is_escaped)
        {
            is_quoted = !is_quoted;

            // quotation mark that terminated string literal.
            if (!is_quoted)
            {
                return i + 1;
            }
        }
        else if (is_escaped)
        {
            is_escaped = false;
        }
    }
    return 0;
}

} // namespace

Tokenizer::Tokenizer(const std::string& string)
    : input(string)
{
}

Token Tokenizer::getNextToken()
{
    if (input.empty())
    {
        return Token{input, TOKEN_END_OF_INPUT};
    }

    const TokenType tokenType = getCharacterTokenType(input.front());
    size_t tokenLen = 0;
    if (input.front() == '"')
    {
        tokenLen = findLengthOfStringLiteral(input);
    }
    else
    {
        const auto p = std::find_if_not(input.cbegin(), input.cend(),
                [tokenType](const char c) -> bool
        {
            return tokenType == getCharacterTokenType(c);
        });

        tokenLen = p - input.begin();
    }

    const Token result{input.substr(0, tokenLen), tokenType};
    input.remove_prefix(tokenLen);

    return result;
}

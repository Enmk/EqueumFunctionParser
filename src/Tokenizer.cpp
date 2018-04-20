/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "Tokenizer.h"

#include <algorithm>
#include <limits>

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
        case '.':
        case ',':
        case ':':
        case ';':
            return TOKEN_PUNCT;
    }
    if (isspace(c))
    {
        return TOKEN_WHITESPACE;
    }
    else if (isdigit(c))
    {
        return TOKEN_NUMBER;
    }
    else
    {
        return TOKEN_STRING;
    }
}

size_t getMaxTokenLength(TokenType type)
{
    const size_t MaxTokenLength = std::numeric_limits<size_t>::max();
    switch(type)
    {
        case TOKEN_WHITESPACE:
        case TOKEN_QUOTED_STRING:
        case TOKEN_STRING:
        case TOKEN_NUMBER:
            return MaxTokenLength;
        case TOKEN_LPAR:
        case TOKEN_RPAR:
        case TOKEN_OP:
        case TOKEN_PUNCT:
            return 1;
        case TOKEN_END_OF_INPUT:
            return 0;
    }
    return MaxTokenLength;
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

Token Tokenizer::peekNextToken() const
{
    if (input.empty())
    {
        return Token{input, TOKEN_END_OF_INPUT};
    }

    TokenType tokenType = getCharacterTokenType(input.front());
    size_t tokenLen = 0;
    if (input.front() == '"')
    {
        tokenType = TOKEN_QUOTED_STRING;
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
    tokenLen = std::min(getMaxTokenLength(tokenType), tokenLen);
    const Token result{input.substr(0, tokenLen), tokenType};

    return result;
}

Token Tokenizer::getNextToken()
{
    const Token result = peekNextToken();
    input.remove_prefix(result.value.length());

    return result;
}

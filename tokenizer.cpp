#include "tokenizer.h"

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
        return TOKEN_WHIESPACE;
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

//void skipsSpace(boost::string_view& v)
//{
//    const auto p = std::find_if_not(v.begin(), v.end(), std::isspace);
//    const auto size = v.end() - p;
//    v.remove_prefix(size);
//}

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
                return i;
            }
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
        return Token{TOKEN_END_OF_INPUT, input};
    }

    TokenType tokenType = getCharacterTokenType(input.front());
    size_t tokenLen = 0;
    if (input.front() == '"')
    {
        tokenLen = findLengthOfStringLiteral(input);
    }
    else
    {
        // Eat characters of same type one by one.
        const auto p = std::find_if(input.begin(), input.end(),
                [tokenType](const char c) -> bool
        {
            return isspace(c) || tokenType == getCharacterTokenType(c);
        });
        tokenLen = input.end() - p;
    }

    const Token result{tokenType, input.substr(0, tokenLen)};
    input.remove_prefix(tokenLen);

    return result;
}

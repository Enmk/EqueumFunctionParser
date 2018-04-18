#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <boost/utility/string_view.hpp>

enum TokenType
{
    TOKEN_WHIESPACE,
    TOKEN_LPAR,
    TOKEN_RPAR,
    TOKEN_OP, // = + - / *
    TOKEN_STRING_LITERAL,
    TOKEN_NUMBER_LITERAL,

    TOKEN_END_OF_INPUT
};

struct Token
{
    TokenType type;
    boost::string_view value;
};

class Tokenizer
{
public:
    explicit Tokenizer(const std::string& string);

    Token getNextToken();
    boost::string_view getRemainder() const;

private:
    boost::string_view input;
};

#endif // TOKENIZER_H

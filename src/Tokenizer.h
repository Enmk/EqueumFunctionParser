/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#ifndef EQUEUM_FUNCTION_PARSER_TOKENIZER_H_INCLUDED
#define EQUEUM_FUNCTION_PARSER_TOKENIZER_H_INCLUDED

#include <boost/version.hpp>

#if BOOST_VERSION < 106000
    #include <boost/utility/string_ref.hpp>
#else
    #include <boost/utility/string_view.hpp
#endif

#include <string>

// Hackety hack, compatibility with boost prior to 1.60.00
#if BOOST_VERSION < 106000
namespace boost
{
using string_view = boost::string_ref;
}
#endif

enum TokenType : int
{
    TOKEN_WHITESPACE, // any whitespace
    TOKEN_LPAR,
    TOKEN_RPAR,
    TOKEN_OP, // = + - / *
    TOKEN_PUNCT, // punctuation marks: .,;:
    TOKEN_QUOTED_STRING, // unquoted string literal
    TOKEN_STRING, // unquoted string literal
    TOKEN_NUMBER, // number without decimal point

    TOKEN_END_OF_INPUT // the last token
};

struct Token
{
    boost::string_view value;
    TokenType type;
};

class Tokenizer
{
public:
    explicit Tokenizer(const std::string& string);

    Token peekNextToken() const;
    Token getNextToken();
//    boost::string_view getRemainder() const;

private:
    boost::string_view input;
};

#endif // EQUEUM_FUNCTION_PARSER_TOKENIZER_H_INCLUDED

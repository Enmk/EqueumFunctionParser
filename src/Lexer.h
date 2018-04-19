/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#ifndef EQUEUM_FUNCTION_PARSER_LEXER_H_INCLUDED
#define EQUEUM_FUNCTION_PARSER_LEXER_H_INCLUDED

#include "Tokenizer.h"

#include <deque>
#include <exception>
#include <string>

struct Token;

enum LexemeType
{
    LEX_NUMBER_LITERAL, // number, integer or with decimal point, with leading '+' or '-'
    LEX_STRING_LITERAL, // quoted string
    LEX_NAME, //
    LEX_OPERATOR, // +, -, *, /, =
    LEX_PUNCTUATION, // ,.:;
    LEX_LEFT_PARENTHESIS, // (
    LEX_RIGHT_PARENTHESIS, // )

    LEX_END_OF_INPUT
};

struct Lexeme
{
    std::string value;
    LexemeType type;
};

//class LexerException : public std::exception
//{
//public:
//    explicit LexerException(std::string message);
//    ~LexerException() override;

//    const char* what() const noexcept override;

//private:
//    const std::string message;
//};

class Tokenizer;

class Lexer
{
public:
    explicit Lexer(const std::string& input);
    ~Lexer();

    Lexeme getNextLexeme();

private:
    Tokenizer tokenizer;
    std::deque<Token> stack;
};

#endif // EQUEUM_FUNCTION_PARSER_LEXER_H_INCLUDED

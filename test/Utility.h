/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#ifndef EQUEUM_FUNCTION_PARSER_TEST_UTILITY_H_INCLUDED
#define EQUEUM_FUNCTION_PARSER_TEST_UTILITY_H_INCLUDED

#include <iosfwd>

struct Lexeme;
struct Token;
struct FunctionCall;
struct FunctionSpecParameter;
struct FunctionCallParameter;
enum TokenType : int;
enum LexemeType : int;

std::ostream& operator<<(std::ostream& ostr, const TokenType& tokenType);
std::ostream& operator<<(std::ostream& ostr, const LexemeType& lexType);
std::ostream& operator<<(std::ostream& ostr, const Token& token);
std::ostream& operator<<(std::ostream& ostr, const Lexeme& lex);
std::ostream& operator<<(std::ostream& ostr, const FunctionSpecParameter& param);
std::ostream& operator<<(std::ostream& ostr, const FunctionCallParameter& param);
std::ostream& operator<<(std::ostream& ostr, const FunctionCall& call);

bool operator==(const Token& left, const Token& right);
bool operator==(const Lexeme& left, const Lexeme& right);
bool operator==(const FunctionSpecParameter& left, const FunctionSpecParameter& right);
bool operator==(const FunctionCallParameter& left, const FunctionCallParameter& right);
bool operator==(const FunctionCall& left, const FunctionCall& right);

#endif // EQUEUM_FUNCTION_PARSER_TEST_UTILITY_H_INCLUDED

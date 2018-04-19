/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "Utility.h"

#include "FunctionParser.h"
#include "Lexer.h"
#include "Tokenizer.h"

#include <boost/preprocessor/stringize.hpp>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <unordered_map>

namespace
{

template <typename T>
bool isEqualParameters(const T& left, const T& right)
{
    return left.name == right.name && left.value == right.value;
}

bool isLessByName(const FunctionCallParameter& left, const FunctionCallParameter& right)
{
    return left.name < right.name;
}

} // namespace

#define TYPE_STRING(T) {T, BOOST_PP_STRINGIZE(T)}


std::ostream& operator<<(std::ostream& ostr, const TokenType& tokenType)
{
    static const std::unordered_map<size_t, const char*> TokenTypeNames =
    {
        TYPE_STRING(TOKEN_WHITESPACE),
        TYPE_STRING(TOKEN_LPAR),
        TYPE_STRING(TOKEN_RPAR),
        TYPE_STRING(TOKEN_OP),
        TYPE_STRING(TOKEN_PUNCT),
        TYPE_STRING(TOKEN_STRING),
        TYPE_STRING(TOKEN_QUOTED_STRING),
        TYPE_STRING(TOKEN_NUMBER),
        TYPE_STRING(TOKEN_END_OF_INPUT),
    };
    return ostr << TokenTypeNames.at(tokenType);
}

std::ostream& operator<<(std::ostream& ostr, const LexemeType& lexType)
{
    static const std::unordered_map<size_t, const char*> LexTypeNames =
    {
        TYPE_STRING(LEX_NUMBER_LITERAL),
        TYPE_STRING(LEX_STRING_LITERAL),
        TYPE_STRING(LEX_NAME),
        TYPE_STRING(LEX_OPERATOR),
        TYPE_STRING(LEX_PUNCTUATION),
        TYPE_STRING(LEX_LEFT_PARENTHESIS),
        TYPE_STRING(LEX_RIGHT_PARENTHESIS),
        TYPE_STRING(LEX_END_OF_INPUT)
    };
    return ostr << LexTypeNames.at(lexType);
}

std::ostream& operator<<(std::ostream& ostr, const Lexeme& lex)
{
    return ostr << "Lexeme{\"" << lex.value << "\", " << lex.type << "}";
}

std::ostream& operator<<(std::ostream& ostr, const Token& token)
{
    return ostr << "Token{\"" << token.value << "\", " << token.type << "}";
}

std::ostream& operator<<(std::ostream& ostr, const FunctionSpecParameter& param)
{
    ostr << param.name;
    if (param.value)
    {
        ostr << "=" << *param.value;
    }

    return ostr;
}

std::ostream& operator<<(std::ostream& ostr, const FunctionCallParameter& param)
{
    if (param.name)
    {
        ostr << *param.name << " = ";
    }

    return ostr << param.value;
}

std::ostream& operator<<(std::ostream& ostr, const FunctionCall& call)
{
    ostr << call.name << "(";
    for (const auto& p : call.parameters)
    {
        ostr << " " << p;

        if (&p != &call.parameters.back())
        {
            ostr << ", ";
        }
    }

    return ostr << ")";
}


bool operator==(const Lexeme& left, const Lexeme& right)
{
    return left.type == right.type && left.value == right.value;
}

bool operator==(const Token& left, const Token& right)
{
    return left.type == right.type && left.value == right.value;
}

bool operator==(const FunctionSpecParameter& left, const FunctionSpecParameter& right)
{
    return isEqualParameters(left, right);
}

bool operator==(const FunctionCallParameter& left, const FunctionCallParameter& right)
{
    return isEqualParameters(left, right);
}

bool operator==(const FunctionCall& left, const FunctionCall& right)
{
    bool result = (left.name == right.name)
            && (left.parameters.size() == right.parameters.size());
    if (!result)
    {
        return result;
    }

    // Copying to do sorting by name.
    auto leftParameters = left.parameters;
    auto rightParameters = right.parameters;

    // stable sort to keep order of positional arguments.
    std::stable_sort(leftParameters.begin(), leftParameters.end(), isLessByName);
    std::stable_sort(rightParameters.begin(), rightParameters.end(), isLessByName);

    auto l = leftParameters.begin();
    auto r = rightParameters.begin();
    const auto left_end = leftParameters.end();
    const auto right_end = rightParameters.end();
    while (result && l != left_end && r != right_end)
    {
        result &= (*l == *r);

        ++l;
        ++r;
    }

    return result;
}

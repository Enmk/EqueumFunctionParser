/*
 * This file is licensed under CC0 license.
 * Author: Vasily Nemkov (v.nemkov@gmail.com)
*/

#include "Lexer.h"

#include "Tokenizer.h"

#include <deque>
#include <vector>

namespace
{

bool isTerminalToken(const Token& token)
{
    return token.type == TOKEN_END_OF_INPUT
            || token.type == TOKEN_WHITESPACE
            || token.type == TOKEN_LPAR
            || token.type == TOKEN_RPAR
            || token.type == TOKEN_QUOTED_STRING
            || token.type == TOKEN_OP
            || (token.type == TOKEN_PUNCT && token.value != ".");
}

LexemeType convertTokenTypeToLexemeType(TokenType tokenType)
{
    switch (tokenType)
    {
        case TOKEN_WHITESPACE:
            assert(false && "Whitespace is not a lexeme");
        case TOKEN_LPAR:
            return LEX_LEFT_PARENTHESIS;
        case TOKEN_RPAR:
            return LEX_RIGHT_PARENTHESIS;
        case TOKEN_OP:
            return LEX_OPERATOR;
        case TOKEN_PUNCT:
            return LEX_PUNCTUATION;
        case TOKEN_QUOTED_STRING:
            return LEX_STRING_LITERAL;
        case TOKEN_STRING:
            return LEX_NAME;
        case TOKEN_NUMBER:
            return LEX_NUMBER_LITERAL;
        case TOKEN_END_OF_INPUT:
            return LEX_END_OF_INPUT;
    }
}

class LexemeBuilder
{
public:
    LexemeBuilder(LexemeType type)
        : type(type)
    {}

    virtual ~LexemeBuilder()
    {}

    /// Returns true when Lexeme can be produced.
    virtual void consumeToken(const Token& token) = 0;

    Lexeme produceLexeme()
    {
        if (!canProduceLexeme)
        {
            assert(false && "LexemeBuilder is not ready to produce a Lexeme"
                    " (not enought input?).");
        }

        std::string value;
        for (const auto& t : accumulatedTokens)
        {
            value += t.value.to_string();
        }

        return Lexeme{value, type};
    }

protected:
    bool canProduceLexeme = false;
    std::deque<Token> accumulatedTokens;
    const LexemeType type;
};

class NameLexemeBuilder : public LexemeBuilder
{
public:
    NameLexemeBuilder()
        : LexemeBuilder(LEX_NAME)
    {
    }

    void consumeToken(const Token& token) override
    {
        if (token.type == TOKEN_STRING || token.type == TOKEN_NUMBER)
        {
            accumulatedTokens.push_back(token);
            canProduceLexeme = true;
        }
        else
        {
            assert(false && "Unsupported token type for LEX_NAME");
        }
    }
};

class NumberLexemeBuilder : public LexemeBuilder
{
public:
    NumberLexemeBuilder()
            : LexemeBuilder(LEX_NUMBER_LITERAL)
    {
    }

    void consumeToken(const Token& token) override
    {
        if (token.type == TOKEN_PUNCT && token.value == ".")
        {
            if (dots_allowed)
            {
                accumulatedTokens.push_back(token);
                --dots_allowed;
                canProduceLexeme = false;
            }
            else
            {
                assert(false && "Multiple decimal dots in a number.");
            }
        }

        if (token.type == TOKEN_NUMBER)
        {
            accumulatedTokens.push_back(token);
            canProduceLexeme = true;
        }
    }
private:
    size_t dots_allowed = 1;
};

LexemeBuilder* makeLexemeBuilder(const Token& token)
{
    switch (token.type)
    {
        case TOKEN_NUMBER:
            return new NumberLexemeBuilder();
        case TOKEN_STRING:
            return new NameLexemeBuilder();
        default:
            assert(false && "No builder for given Token type.");
    }
    return nullptr;
}

Lexeme buildLexeme(std::deque<Token>& tokens)
{
    if (tokens.empty())
    {
        assert(false && "Can't build Lexeme from empty tokens stack.");
    }

    Token token = tokens.front();
    tokens.pop_front();
    if (isTerminalToken(token))
    {
        return Lexeme{token.value.to_string(), convertTokenTypeToLexemeType(token.type)};
    }

    std::unique_ptr<LexemeBuilder> lexemeBuilder(makeLexemeBuilder(token));
    do
    {
        lexemeBuilder->consumeToken(token);
        if (tokens.empty() || isTerminalToken(tokens.front()))
        {
            break;
        }

        token = tokens.front();
        tokens.pop_front();
    }
    while (true);

    return lexemeBuilder->produceLexeme();
}

} // namespace

//LexerException::LexerException(std::string message)
//    : message(std::move(message))
//{}

//LexerException::~LexerException()
//{}

//const char* LexerException::what() const noexcept
//{
//    return message.c_str();
//}

Lexer::Lexer(const std::string& input)
    : tokenizer(input)
{}

Lexer::~Lexer()
{}

Lexeme Lexer::getNextLexeme()
{
    // For all tokens in a tokenizer:
    //    if token is terminal:
    //        produce lexeme from current stack
    //        put token to stack
    //        return
    if (!stack.empty())
    {
        return buildLexeme(stack);
    }

    Token token = tokenizer.getNextToken();
    while(token.type != TOKEN_END_OF_INPUT)
    {
        if (token.type != TOKEN_WHITESPACE)
        {
            stack.push_back(token);
        }

        if (isTerminalToken(token)
            // skipping initial whitespace
            && !(stack.empty() && token.type == TOKEN_WHITESPACE))
        {
            break;
        }

        token = tokenizer.getNextToken();
    }

    if (stack.empty()
            && (token.type == TOKEN_END_OF_INPUT
                || token.type == TOKEN_WHITESPACE))
    {
        return Lexeme{std::string(), LEX_END_OF_INPUT};
    }

    return buildLexeme(stack);
}

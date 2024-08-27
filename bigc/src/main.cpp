#include <iostream>
#include "Expression.h"
#include <stack>
#include <vector>
#include "Token.h"

const std::string OPERATORS = "+-*/!=><";
const std::string DIGITS = "0123456789";
const std::string WORDS = "abcdefghijklmnopqrstuvwxyz_";

template <class T>
struct Result
{
    T value;
    std::string error = "";

    Result<T>(T val)
    {
        value = val;
    }

    Result<T>(std::string str)
    {
        error = str;
    }
};

Result<TokenType> getTokenType(char c, TokenType prev, std::string acc)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return NONE;
    switch (prev)
    {
    case NUMBER:
        bool hasDot = acc.find('.') != std::string::npos;
        if (c == '.' && hasDot)
            return Result<TokenType>("why dot");
        if (DIGITS.find(c) != std::string::npos || (!hasDot && c == '.'))
            return Result<TokenType>(NUMBER);
        return Result<TokenType>("invalid number");
    case TEXT:
        if (WORDS.find(c) != std::string::npos || DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        if (c == '.')
            return Result<TokenType>(ACCESSOR);
        if (c == ',')
            return Result<TokenType>(DELIMITER);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        return Result<TokenType>("invalid word");
    case NONE:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        else if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        else if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
    case ACCESSOR:
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        return Result<TokenType>("expected property");
    }
    return Result<TokenType>("invalid token");
}

Result<std::vector<Token>> tokenize(std::string str)
{
    std::vector<Token> tokens;
    std::string accumulated = "";
    TokenType accType = NONE;
    for (int i = 0; i < str.size(); i++)
    {
        char c = str[i];
        Result<TokenType> result = getTokenType(c, accType, accumulated);
        if (!result.error.empty())
        {
            return Result<std::vector<Token>>(result.error);
        }
        if (result.value == NONE)
        {
        }
        if (result.value != accType)
        {
            tokens.push_back(Token(accumulated, accType));
            accumulated = "";
        }
        accType = result.value;
        accumulated += c;
    }
    for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens[i].value << '\n';
    }
    return Result<std::vector<Token>>(tokens);
}

/*for (int i = index; i < expr.length(); i++)
{
    char c = expr[i];
    if (DIGITS.find(c) != std::string::npos)
    {
        if (!accumulated.empty() && accumulatedType != NUMBER)
        {
            tokens.push_back(Token(accumulated, accumulatedType));
            accumulated = "";
        }
        accumulated += c;
        accumulatedType = NUMBER;
    }
    else if (OPERATORS.find(c) != std::string::npos)
    {
        if (!accumulated.empty() && accumulatedType != OPERATOR)
        {
            tokens.push_back(Token(accumulated, accumulatedType));
            accumulated = "";
        }
        accumulated += c;
        accumulatedType = OPERATOR;
    }
    else if (VARS.find(c) != std::string::npos)
    {
        if (!accumulated.empty() && accumulatedType != VARIABLE)
        {
            tokens.push_back(Token(accumulated, accumulatedType));
        }
        accumulated += c;
        accumulatedType = VARIABLE;
    }
    /*else if (c == '(')
    {
        tokens.push_back(interpretExpr(expr, ++i));
    }
    else if (c == ')')
    {
        i++;
        break;
    }
    else if (c == '.')
    {
        if (!accumulated.empty() && accumulatedType != VARIABLE)
        {
            tokens.push_back(Token(accumulated, accumulatedType));
        }
        tokens.push_back(Token(".", VARIABLE));
    }
    else if (c == '.')
    {
        if (!accumulated.empty() && accumulatedType != VARIABLE)
        {
            tokens.push_back(Token(accumulated, accumulatedType));
        }
        tokens.push_back(Token(".", VARIABLE));
    }
}
    */

int main(int argc, char *argv[])
{
    std::string line = "";
    for (int i = 1; i < argc; i++)
    {
        line += argv[i];
        line += " ";
    }
    tokenize(line);
    return 0;
}
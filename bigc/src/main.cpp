#include <iostream>
#include "Expression.h"
#include <stack>
#include <vector>
#include "Token.h"

const std::string OPERATORS = "+-*!/=><";
const std::string DIGITS = "0123456789";
const std::string WORDS = "abcdefghijklmnopqrstuvwxyz_";
const std::string SINGULARS = ".,()[]{}|&~;";

template <class T>
struct Result
{
    T value;
    std::string error;

    Result<T>(T val)
    {
        value = val;
        error = "";
    }

    Result<T>(std::string str)
    {
        error = str;
    }
};

Result<TokenType> allowSingularTokens(char c, TokenType allowed[12], std::string error) {
    for (int i = 0; i < 12; i++) {
        char token = SINGULARS[i];
        if (allowed[i] != NONE && c == token) {
            return allowed[i];
        }
    }
    return Result<TokenType>(error);
}

Result<TokenType> getTokenType(char c, TokenType prev, std::string acc, bool prevWhitespace)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return Result<TokenType>(NONE);
    bool hasDot = acc.find('.') != std::string::npos;
    TokenType allowed[12] = {
        ACCESSOR,
        DELIMITER,
        ARGEXPRSTART,
        ARGEXPREND,
        INDSTART,
        INDEND,
        CTRLSTART,
        CTRLEND,
        PIPE,
        PIPERES,
        SPREAD,
        END
    };

    switch (prev)
    {
    case NONE:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLSTART - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[PIPERES - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected start of statement");

    case NUMBER:
        if (c == '.' && hasDot)
            return Result<TokenType>("why dot");
        if (DIGITS.find(c) != std::string::npos || (!hasDot && c == '.'))
            return Result<TokenType>(NUMBER);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[ARGEXPRSTART - 4] = NONE;
        allowed[INDSTART - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[PIPERES - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "invalid token after number");
    case TEXT:
        if (prevWhitespace && DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos || DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[PIPERES - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "invalid token after word");
    case OPERATOR:
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLSTART - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        return allowSingularTokens(c, allowed, "invalid token after operator");
    case ACCESSOR:
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        return Result<TokenType>("expected property");
    case DELIMITER:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ACCESSOR - 4] = NONE;
        allowed[DELIMITER - 4] = NONE;
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "expected expression");
    case ARGEXPRSTART:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ACCESSOR - 4] = NONE;
        allowed[DELIMITER - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "expected value");
    case ARGEXPREND:
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[PIPERES - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected post-expression token");
    case INDSTART:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[PIPE - 4] = NONE;
        allowed[ARGEXPREND - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "");
    case INDEND:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[PIPERES - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected token after ]");
    case CTRLSTART:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ACCESSOR - 4] = NONE;
        allowed[DELIMITER - 4] = NONE;
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLSTART - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected token after control");
    case CTRLEND:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[PIPERES - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected post-expression token");
    case PIPE:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ACCESSOR - 4] = NONE;
        allowed[DELIMITER - 4] = NONE;
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "expected expression after pipe");
    case PIPERES:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected token after pipe result");
    case SPREAD:
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLSTART - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "expected expression to spread");
    default:
    return Result<TokenType>("invalid token");
    }

    return Result<TokenType>("invalid token");
}

Result<std::vector<Token> > tokenize(std::string str)
{
    std::vector<Token> tokens;
    std::string accumulated = "";
    TokenType accType = NONE;
    bool prevWhitespace = false;
    for (int i = 0; i < str.size(); i++)
    {
        char c = str[i];
        Result<TokenType> result = getTokenType(c, accType, accumulated, prevWhitespace);
        if (!result.error.empty())
        {
            std::string msg = ("Column " + std::to_string(i + 1) + "\n" + result.error);
            msg = "Near \"" + str.substr(std::max(0, i - 3), 5) + "\"\n" + msg;
            return Result<std::vector<Token> >(msg);
        }
        if (result.value == NONE)
        {
            prevWhitespace = true;
            continue;
        }
        prevWhitespace = false;
        if (result.value != accType)
        {
            tokens.push_back(Token(accumulated, accType));
            accumulated = "";
        }
        accType = result.value;
        accumulated += c;
    }
    if (!accumulated.empty())
        tokens.push_back(Token(accumulated, accType));
    for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens[i].value << '\n';
    }
    return Result<std::vector<Token> >(tokens);
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
    else if (c == '(')
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
    Result<std::vector<Token> > result = tokenize(line);
    if (!result.error.empty())
        std::cerr << result.error << '\n';
    return 0;
}

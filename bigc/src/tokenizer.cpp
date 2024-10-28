#include "tokenizer.h"

namespace tokenizer
{
    const std::string OPERATORS = "+-*!/=><@";
    const std::string DIGITS = "0123456789";
    const std::string WORDS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_:";
    const std::string SINGULARS = ".,()[]{}|&~;";

    Result<TokenType> allowSingularTokens(char c, TokenType allowed[12], std::string error)
    {
        for (int i = 0; i < 12; i++)
        {
            char token = SINGULARS[i];
            if (allowed[i] != NONE && c == token)
            {
                return allowed[i];
            }
        }
        return Result<TokenType>(error);
    }

    /* POST-CHARACTER MATRIX: x means disallowed
                a 0 + . , ( ) [ ] { } | & ~ ;
    NONE              x x   x   x x x x x   x
    TEXT                                x x
    NUMBERSTR     x x   x   x   x         x x
    ACCESSOR      x x x x x x x x x x x x x x
    DELIMITER         x x   x   x x x x     x
    ARGEXSTART        x x       x x x x     x
    ARGEXEND    x x                     x x
    INDSTART          x x   x   x x x x     x
    INDEND      x x                     x
    CTRLSTART         x x   x   x x x x     x
    CTRLEND     x x                     x x
    PIPE              x x   x   x x x x     x
    PIPERES     x x                       x
    SPREAD        x x x x   x   x x x x     x

    */

    Result<TokenType> getTokenType(char c, TokenType prev, std::string acc, bool prevWhitespace)
    {
        if (c == ' ' || c == '\t')
            return Result<TokenType>(NONE);
        if (c == '\n')
            return Result<TokenType>(SOFTEND);
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
            END};

        switch (prev)
        {
        case END:
            if (OPERATORS.find(c) != std::string::npos)
                return Result<TokenType>(OPERATOR);
            if (DIGITS.find(c) != std::string::npos)
                return Result<TokenType>(NUMBERSTR);
            if (WORDS.find(c) != std::string::npos)
                return Result<TokenType>(TEXT);
            allowed[ACCESSOR - 4] = NONE;
            allowed[DELIMITER - 4] = NONE;
            allowed[ARGEXPREND - 4] = NONE;
            allowed[INDEND - 4] = NONE;
            allowed[PIPE - 4] = NONE;
            allowed[PIPERES - 4] = NONE;
            return allowSingularTokens(c, allowed, "unexpected end of statement");

        case NONE:
            if (OPERATORS.find(c) != std::string::npos)
                return Result<TokenType>(OPERATOR);
            if (DIGITS.find(c) != std::string::npos)
                return Result<TokenType>(NUMBERSTR);
            if (WORDS.find(c) != std::string::npos)
                return Result<TokenType>(TEXT);
            allowed[ACCESSOR - 4] = NONE;
            allowed[DELIMITER - 4] = NONE;
            allowed[ARGEXPREND - 4] = NONE;
            allowed[INDEND - 4] = NONE;
            allowed[CTRLSTART - 4] = NONE;
            allowed[CTRLEND - 4] = NONE;
            allowed[PIPE - 4] = NONE;
            allowed[PIPERES - 4] = NONE;
            allowed[END - 4] = NONE;
            return allowSingularTokens(c, allowed, "unexpected start of statement");

        case SOFTEND:
            if (OPERATORS.find(c) != std::string::npos)
                return Result<TokenType>(OPERATOR);
            if (DIGITS.find(c) != std::string::npos)
                return Result<TokenType>(NUMBERSTR);
            if (WORDS.find(c) != std::string::npos)
                return Result<TokenType>(TEXT);
            return allowSingularTokens(c, allowed, "unexpected token after soft end");

        case NUMBERSTR:
            if (c == '.' && hasDot)
                return Result<TokenType>("why dot");
            if (DIGITS.find(c) != std::string::npos || (!hasDot && c == '.'))
                return Result<TokenType>(NUMBERSTR);
            if (OPERATORS.find(c) != std::string::npos)
                return Result<TokenType>(OPERATOR);
            allowed[ARGEXPRSTART - 4] = NONE;
            allowed[INDSTART - 4] = NONE;
            allowed[PIPERES - 4] = NONE;
            allowed[SPREAD - 4] = NONE;
            return allowSingularTokens(c, allowed, "invalid token after number or string");
        case TEXT:
            if (prevWhitespace && DIGITS.find(c) != std::string::npos)
                return Result<TokenType>(NUMBERSTR);
            if (WORDS.find(c) != std::string::npos || DIGITS.find(c) != std::string::npos)
                return Result<TokenType>(TEXT);
            if (OPERATORS.find(c) != std::string::npos)
                return Result<TokenType>(OPERATOR);
            allowed[PIPERES - 4] = NONE;
            allowed[SPREAD - 4] = NONE;
            return allowSingularTokens(c, allowed, "invalid token after word");
        case OPERATOR:
            if (DIGITS.find(c) != std::string::npos)
                return Result<TokenType>(NUMBERSTR);
            if (WORDS.find(c) != std::string::npos)
                return Result<TokenType>(TEXT);
            if (OPERATORS.find(c) != std::string::npos)
                return Result<TokenType>(OPERATOR);
            allowed[ARGEXPREND - 4] = NONE;
            allowed[INDEND - 4] = NONE;
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
                return Result<TokenType>(NUMBERSTR);
            if (WORDS.find(c) != std::string::npos)
                return Result<TokenType>(TEXT);
            allowed[ACCESSOR - 4] = NONE;
            allowed[DELIMITER - 4] = NONE;
            allowed[INDEND - 4] = NONE;
            allowed[CTRLSTART - 4] = NONE;
            allowed[CTRLEND - 4] = NONE;
            allowed[PIPE - 4] = NONE;
            allowed[END - 4] = NONE;
            return allowSingularTokens(c, allowed, "expected expression");
        case ARGEXPRSTART:
            if (OPERATORS.find(c) != std::string::npos)
                return Result<TokenType>(OPERATOR);
            if (DIGITS.find(c) != std::string::npos)
                return Result<TokenType>(NUMBERSTR);
            if (WORDS.find(c) != std::string::npos)
                return Result<TokenType>(TEXT);
            allowed[ACCESSOR - 4] = NONE;
            allowed[DELIMITER - 4] = NONE;
            allowed[INDEND - 4] = NONE;
            allowed[CTRLSTART - 4] = NONE;
            allowed[CTRLEND - 4] = NONE;
            allowed[PIPE - 4] = NONE;
            allowed[END - 4] = NONE;
            return allowSingularTokens(c, allowed, "expected value");
        case ARGEXPREND:
            /*if (DIGITS.find(c) != std::string::npos)
                return Result<TokenType>(NUMBERSTR);
            */
            if (WORDS.find(c) != std::string::npos)
                return Result<TokenType>(TEXT);
            if (OPERATORS.find(c) != std::string::npos)
                return Result<TokenType>(OPERATOR);
            allowed[PIPERES - 4] = NONE;
            allowed[SPREAD - 4] = NONE;
            return allowSingularTokens(c, allowed, "unexpected post-expression token");
        case INDSTART:
            if (OPERATORS.find(c) != std::string::npos)
                return Result<TokenType>(OPERATOR);
            if (DIGITS.find(c) != std::string::npos)
                return Result<TokenType>(NUMBERSTR);
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
                return Result<TokenType>(NUMBERSTR);
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
            if (WORDS.find(c) != std::string::npos)
                return Result<TokenType>(TEXT);
            allowed[PIPERES - 4] = NONE;
            allowed[SPREAD - 4] = NONE;
            return allowSingularTokens(c, allowed, "unexpected post-expression token");
        case PIPE:
            if (OPERATORS.find(c) != std::string::npos)
                return Result<TokenType>(OPERATOR);
            if (DIGITS.find(c) != std::string::npos)
                return Result<TokenType>(NUMBERSTR);
            if (WORDS.find(c) != std::string::npos)
                return Result<TokenType>(TEXT);
            allowed[ACCESSOR - 4] = NONE;
            allowed[DELIMITER - 4] = NONE;
            allowed[ARGEXPREND - 4] = NONE;
            allowed[INDEND - 4] = NONE;
            allowed[CTRLSTART - 4] = NONE;
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
            allowed[CTRLEND - 4] = NONE;
            allowed[PIPE - 4] = NONE;
            allowed[SPREAD - 4] = NONE;
            allowed[END - 4] = NONE;
            return allowSingularTokens(c, allowed, "expected expression to spread");
        default:
            return Result<TokenType>("invalid token");
        }
    }

    Result<std::vector<Token>> tokenize(std::string str)
    {
        std::vector<Token> tokens;
        std::string accumulated = "";
        TokenType accType = NONE;
        bool prevWhitespace = false;
        bool inString = false;
        for (int i = 0; i < str.size(); i++)
        {
            char c = str[i];
            // handle string
            if (c == '"' && !inString)
            {
                if (!accumulated.empty())
                    tokens.push_back(Token(accumulated, accType));
                accumulated = "";
                inString = true;
                accType = NUMBERSTR;
                continue;
            }
            else if (c == '"' && inString && str[i - 1] != '\\')
            {
                inString = false;
                // differenatiate between strs and numbers
                tokens.push_back(Token('s' + accumulated, NUMBERSTR));
                accumulated = "";
                continue;
            }
            else if (inString)
            {
                if (c != '\\')
                    accumulated += c;
                continue;
            }
            Result<TokenType> result = getTokenType(c, accType, accumulated, prevWhitespace);
            if (!result.ok())
            {
                std::string msg = ("Column " + std::to_string(i + 1) + "\n" + result.getError());
                msg = "Near \"" + str.substr(std::max(0, i - 3), 5) + "\"\n" + msg;
                return Result<std::vector<Token>>(msg);
            }
            if (result.getValue() == NONE)
            {
                prevWhitespace = true;
                if (!accumulated.empty())
                {
                    if (accType == NUMBERSTR)
                    {
                        if (accumulated.find('.') == std::string::npos)
                            accumulated = 'n' + accumulated;
                        else
                            accumulated = 'f' + accumulated;
                    }
                    tokens.push_back(Token(accumulated, accType));
                    accumulated = "";
                }
                continue;
            }
            prevWhitespace = false;
            if ((result.getValue() != accType || (SINGULARS.find(c) != std::string::npos && c != '.')) && accType != NONE && !accumulated.empty())
            {

                if (accType == NUMBERSTR)
                {
                    if (accumulated.find('.') == std::string::npos)
                        accumulated = 'n' + accumulated;
                    else
                        accumulated = 'f' + accumulated;
                }
                tokens.push_back(Token(accumulated, accType));
                accumulated = "";
            }
            accType = result.getValue();
            accumulated += c;
        }
        if (!accumulated.empty())
        {
            // differentiate between numbers and strs
            if (accType == NUMBERSTR)
                accumulated = 'n' + accumulated;
            tokens.push_back(Token(accumulated, accType));
        }
        return Result<std::vector<Token>>(tokens);
    }
}

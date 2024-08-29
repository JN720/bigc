#pragma once
#include <string>

enum TokenType
{
    NONE,
    NUMBER,
    OPERATOR,
    TEXT,
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

class Token
{
public:
    std::string value;
    TokenType type;
    Token(const std::string value, TokenType type);
};

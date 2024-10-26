#pragma once
#include <string>

const std::string TOKEN_TYPES[] = {
    "none",
    "number",
    "operator",
    "text",
    "accessor",
    "delimiter",
    "argexprstart",
    "argexprend",
    "indstart",
    "indend",
    "ctrlstart",
    "ctrlend",
    "pipe",
    "piperes",
    "spread",
    "end"};

enum TokenType
{
    NONE,
    NUMBERSTR,
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

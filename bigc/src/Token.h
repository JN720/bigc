#include <string>

enum TokenType
{
    NONE,
    NUMBER,
    OPERATOR,
    TEXT,
    ACCESSOR,
    DELIMITER,
    EXPRSTART,
    EXPREND,
    ARGSTART,
    ARGEND,
    CTRLSTART,
    CTRLEND,
    ARRSTART,
    ARREND,
    INDSTART,
    INDEND,
    PIPE,
    PIPERES,
    SPREAD
};

class Token
{
public:
    std::string value;
    TokenType type;
    Token(const std::string value, TokenType type);
};
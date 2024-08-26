#include <string>

enum TokenType
{
    NUMBER,
    OPERATOR,
    VARIABLE
};

class Token
{
public:
    std::string value;
    TokenType type;
    Token(const std::string value, TokenType type);
};
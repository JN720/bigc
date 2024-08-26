#include "Token.h"

Token::Token(const std::string value, TokenType type)
{
    this->value = value;
    this->type = type;
}
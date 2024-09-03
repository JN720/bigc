#include "Value.h"

Value::Value()
{
}

Value::Value(Token &token)
{
    switch (token.type)
    {
    case NUMBERSTR:
        switch (token.value[0])
        {
        case 's':
            value = &token.value.substr(1);
            type = "str";
        case 'n':
            value = (void *)(std::atoi(token.value.substr(1).c_str()));
            type = "num";
        default:
            throw "unrecognized starting character for value";
        }
    default:
        throw "invalid token type for a value Node";
    }
}

Value::Value(std::string type)
{
    this->type = type;
}

Value::Value(std::string type, void *value)
{
    this->type = type;
    this->value = value;
}

void *Value::getValue()
{
    return value;
}

std::string Value::getType()
{
    return type;
}
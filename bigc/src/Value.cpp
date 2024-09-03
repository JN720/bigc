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
            value = new std::string(token.value.substr(1));
        case 'n':
            value = new int(std::atoi(token.value.substr(1).c_str()));
        default:
            throw "unrecognized starting character for value";
        }
    default:
        throw "invalid token type for a value Node";
    }
}

Value::Value(Wildcard value)
{
    this->value = value;
}

Wildcard Value::getValue()
{
    return value;
}

std::string Value::getType()
{
    if (std::holds_alternative<int *>(value))
        return "int";
    else if (std::holds_alternative<float *>(value))
        return "float";
    else if (std::holds_alternative<char *>(value))
        return "char";
    else if (std::holds_alternative<std::string *>(value))
        return "str";
    else if (std::holds_alternative<Array *>(value))
        return "arr";
    else if (std::holds_alternative<Node *>(value))
        return "fn";
    else if (std::holds_alternative<Object *>(value))
        return "obj";
    return "nil";
}

Result<Value> Value::add(Value other)
{
    return Value();
}

Result<Value> Value::negate()
{
    return Value();
}
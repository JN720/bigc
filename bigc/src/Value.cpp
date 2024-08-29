#include "Value.h"

Value::Value()
{
}

Value::Value(Token &token)
{
    value = (void *)&token.value;
}

Value::Value(std::string type)
{
    this->type = type;
}

void *Value::getValue(const State &state)
{
    return this->value;
}

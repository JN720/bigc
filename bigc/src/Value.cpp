#pragma once
#include "Value.h"

Value::Value(Token &token) {
    value = (void*) &token.value;
}

void *Value::getValue()
{
    return this->value;
}

#pragma once
#include <string>
#include "Token.h"
#include "State.h"

class Value
{
public:
    Value();
    Value(Token &token);
    Value(std::string type);
    void *getValue(const State &state);

protected:
    std::string type;
    void *value;
};

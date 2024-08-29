#pragma once
#include <string>
#include "Token.h"

class Value
{
public:
    Value();
    Value(Token &token);
    Value(std::string type);
    void *getValue();

protected:
    std::string type;
    void *value;
};

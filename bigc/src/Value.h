#pragma once
#include <string>
#include "Token.h"

class Value
{
public:
    Value();
    // infer value from token
    Value(Token &token);
    Value(std::string type);
    Value(std::string type, void *value);
    void *getValue();
    std::string getType();

protected:
    std::string type;
    void *value;
};

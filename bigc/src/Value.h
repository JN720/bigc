#pragma once
#include<string>
#include "Token.h"

class Value {
public:
    Value(Token &token);
    void *getValue();
protected:
    std::string type;
    void *value;
};

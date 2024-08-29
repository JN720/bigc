#pragma once
#include <unordered_map>
#include "Value.h"

class State
{
public:
    State();
    void setVariable(std::string name, Value value);
    Value getVariable(std::string name) const;
    bool isKeyword(std::string word);
    bool isFunction(std::string word);

private:
    std::unordered_map<std::string, Value> variables;
};

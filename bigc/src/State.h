#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Value.h"

class State
{
public:
    State();
    void setVariable(std::string name, Value value);
    Value getVariable(std::string name) const;
    bool isKeyword(std::string word);
    bool isFunction(std::string word);
    bool implements(std::string type, std::string interface);

private:
    // currently assigned variables
    std::unordered_map<std::string, Value> variables;
    // the interfaces each type implements
    std::unordered_map<std::string, std::unordered_set<std::string>> types;
    // whate ach interface must have
    std::unordered_map<std::string, std::unordered_set<std::string>> interfaces;
};

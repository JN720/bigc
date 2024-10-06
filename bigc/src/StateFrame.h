#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Value.h"

class ClassNode;

class StateFrame
{
public:
    StateFrame();
    void setVariable(std::string name, Value value);
    Result<Value> getVariable(std::string name) const;
    void listVars();

private:
    // currently assigned variables
    std::unordered_map<std::string, Value> variables;
};
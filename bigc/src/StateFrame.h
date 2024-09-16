#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Value.h"
#include "ClassDefinition.h"

class StateFrame
{
public:
    StateFrame();
    void setVariable(std::string name, Value value);
    Result<Value> getVariable(std::string name) const;
    bool isType(std::string word);
    Result<bool> implements(std::string type, std::string interface);

private:
    // currently assigned variables
    std::unordered_map<std::string, Value> variables;
    // the interfaces each type implements
    std::unordered_map<std::string, std::unordered_set<std::string>> implementedInterfaces;
    // what each interface must have
    std::unordered_map<std::string, std::unordered_set<std::string>> interfaces;
    // class definitions
    std::unordered_map<std::string, ClassDefinition *> classes;
};
#pragma once

#include <unordered_map>
#include <string>
#include "Result.h"
#include "Value.h"

class Registry
{
public:
    void registerVariable(std::string name, Value value);
    void registerVariable(std::string name, Value value, std::string group);
    const Result<Value> &getVariable(std::string name) const;
    const Result<Value> &getVariable(std::string name, std::string group) const;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, Value>> groups;
};
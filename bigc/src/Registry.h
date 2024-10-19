#pragma once

#include <unordered_map>
#include <string>
#include "Result.h"
#include "Value.h"

class Registry
{
public:
    Registry();
    void registerVariable(std::string name, Value value);
    const Result<Value> getVariable(std::string name) const;
    size_t size() const;

private:
    std::unordered_map<std::string, Value> variables;
};
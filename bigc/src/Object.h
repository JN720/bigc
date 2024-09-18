#pragma once
#include <unordered_map>
#include "Value.h"
#include "State.h"

class Object
{
public:
    Object(ClassDefinition *definition);

protected:
    std::unordered_map<std::string, Value> attributes;
};
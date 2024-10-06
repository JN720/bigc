#pragma once
#include <unordered_map>
#include "Value.h"

class ClassDefinition;

class Object
{
public:
    Object(ClassDefinition *definition);
    ClassDefinition *getClass();

protected:
    std::unordered_map<std::string, Value> attributes;
    ClassDefinition *objClass;
};
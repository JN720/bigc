#pragma once
#include <unordered_map>
#include "Value.h"
#include "State.h"
#include "Node.h"
#include <vector>
#include "ClassDefinitionInterface.h"

class Object
{
public:
    Object();
    Object(ClassDefinitionInterface *definition);
    ClassDefinitionInterface *getClass();
    virtual Control setProperty(const std::string &property, const Value &value);
    void addProperty(const std::string &property, const Value &value);
    virtual Result<Value> getProperty(const std::string &property);

protected:
    std::unordered_map<std::string, Value> attributes;
    ClassDefinitionInterface *objClass;
};
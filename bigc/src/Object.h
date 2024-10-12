#pragma once
#include <unordered_map>
#include "Value.h"
#include "State.h"
#include "Node.h"
#include <vector>

class ClassDefinition;

class Object
{
public:
    Object(ClassDefinition *definition);
    ClassDefinition *getClass();
    Result<Value> callMethod(State &state, const std::vector<Node *> &args);
    Control setProperty(const std::string &property, const Value &value);
    void addProperty(const std::string &property, const Value &value);
    Result<Value> getProperty(const std::string &property);

protected:
    std::unordered_map<std::string, Value> attributes;
    ClassDefinition *objClass;
};
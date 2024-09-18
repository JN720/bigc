#pragma once
#include <unordered_map>
#include <string>
#include <unordered_set>
#include "Result.h"
#include "Value.h"

class Node;

class ClassDefinition
{
public:
    ClassDefinition();
    std::string getParentClassName();
    Result<Node *> getMethod(std::string name);

private:
    // implemented interfaces
    std::unordered_set<std::string> interfacesValue;
    std::string parentClass;
    // changeable attribute definitions
    // when checking inheritance, we traverse the chain
    // these should have public:, private:, or protected: at the beginning
    std::unordered_map<std::string, Node *> definitions;
    // non-changeable methods
    // these cannot be changed, though an attribute can be a function
    std::unordered_map<std::string, Node *> methods;
};
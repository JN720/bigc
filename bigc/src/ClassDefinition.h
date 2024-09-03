#pragma once
#include <unordered_map>
#include <string>
#include <unordered_set>

class Node;

class ClassDefinition
{
public:
    ClassDefinition();

private:
    // implemented interfaces
    std::unordered_set<std::string> interfacesValue;
    std::string parentClass;
    // attribute and method definitions
    // these should have public:, private:, or protected: at the beginning
    std::unordered_map<std::string, Node *> definitions;
};
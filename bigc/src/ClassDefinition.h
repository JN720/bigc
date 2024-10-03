#pragma once
#include <unordered_map>
#include <string>
#include <unordered_set>
#include "Result.h"
#include "Value.h"

/*
Example Class:

Dog = class Animal, INoisemaker {
    public name@str;
    public height;
    private age@int;
    protected oerform_trick@fn

    publicm Dog() {
        this.age = 10;
    }

    privatem internal_method() {

    }
}

*/

class Node;

class ClassDefinition
{
public:
    ClassDefinition();
    ClassDefinition *getParent();
    Result<Node *> getMethod(std::string name);

private:
    // implemented interfaces
    std::unordered_set<std::string> interfacesValue;
    ClassDefinition *parentClass;
    // changeable attribute definitions
    // when checking inheritance, we traverse the chain
    // these should have public:, private:, or protected: at the beginning
    std::unordered_map<std::string, Node *> definitions;
    // non-changeable methods
    // these cannot be changed, though an attribute can be a function
    std::unordered_map<std::string, Node *> methods;
};
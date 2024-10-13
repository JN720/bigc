#pragma once
#include <unordered_map>
#include <string>
#include <unordered_set>
#include "Result.h"
#include "Value.h"
#include "Interface.h"
#include "Object.h"

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

enum AccessSpecifier
{
    PUBLIC,
    PRIVATE,
    PROTECTED
};

class Node;
class State;

class ClassDefinition
{
public:
    ClassDefinition();
    ClassDefinition *getParent();
    Result<Node *> getMethod(std::string name);
    void applyParent(ClassDefinition *parent);
    const std::unordered_map<std::string, Node *> &getMethods();
    const std::unordered_map<std::string, Node *> &getStaticMethods();
    const std::unordered_map<std::string, AccessSpecifier> &getAttributes();
    const std::unordered_map<std::string, AccessSpecifier> &getStaticAttributeAccess();
    const std::unordered_map<std::string, Value> &getStaticAttributes();
    const std::unordered_set<Interface *> &getInterfaces();
    void applyInterface(Interface *interface);
    void addMethod(std::string name, Node *method, bool isStatic);
    void addAttribute(std::string name, AccessSpecifier access, bool isStatic);
    bool implements(Interface *interface);
    Result<Node *> getClassMethod(std::string name);
    bool hasAttribute(std::string name);
    Result<Value> construct(State *state, std::vector<Node *> &args);
    Result<Node *> getStaticMethod(std::string name);
    Result<Value> getStaticAttribute(std::string name);
    void addStaticAttribute(std::string name, Value value, AccessSpecifier access);

private:
    // implemented interfaces
    std::unordered_set<Interface *> interfaces;
    ClassDefinition *parentClass;
    // changeable attribute definitions
    // when checking inheritance, we traverse the chain
    // these should have public:, private:, or protected: at the beginning
    std::unordered_map<std::string, AccessSpecifier> attributes;
    // non-changeable methods
    // these cannot be changed, though an attribute can be a function
    std::unordered_map<std::string, Node *> methods;
    // static versions
    std::unordered_map<std::string, AccessSpecifier> staticAttributeAccess;
    std::unordered_map<std::string, Value> staticAttributes;
    std::unordered_map<std::string, Node *> staticMethods;
};
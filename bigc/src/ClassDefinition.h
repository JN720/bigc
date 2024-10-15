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
    protected perform_trick@fn

    public method Dog() {
        this.age = 10;
    }

    private method internal_method() {

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

struct Method
{
    Node *method;
    AccessSpecifier access;
    Method() {}
    Method(Node *method, AccessSpecifier access)
    {
        this->method = method;
        this->access = access;
    }
};

struct Attribute
{
    Value value;
    AccessSpecifier access;
    Value defaultValue;
    Attribute() {}
    Attribute(Value value, AccessSpecifier access, Value defaultValue)
    {
        this->value = value;
        this->access = access;
        this->defaultValue = defaultValue;
    }
};

class ClassDefinition
{
public:
    ClassDefinition();
    ClassDefinition *getParent();
    Result<Node *> getMethod(std::string name);
    void applyParent(ClassDefinition *parent);
    const std::unordered_map<std::string, Method> &getMethods();
    const std::unordered_map<std::string, Method> &getStaticMethods();
    const std::unordered_map<std::string, Attribute> &getAttributes();
    const std::unordered_map<std::string, Attribute> &getStaticAttributes();
    const std::unordered_set<Interface *> &getInterfaces();
    const std::unordered_map<std::string, Attribute> &getAttributeDefaults();
    void applyInterface(Interface *interface);
    void addMethod(std::string name, Node *method, bool isStatic, AccessSpecifier access);
    void addAttribute(std::string name, AccessSpecifier access, bool isStatic, Value defaultValue);
    bool implements(Interface *interface);
    Result<Node *> getClassMethod(std::string name);
    bool hasAttribute(std::string name);
    virtual Result<Value> construct(State *state, std::vector<Node *> &args);
    Result<Node *> getStaticMethod(std::string name);
    Result<Value> getStaticAttribute(std::string name);
    void setStaticAttribute(std::string name, Value value);

private:
    // implemented interfaces
    std::unordered_set<Interface *> interfaces;
    ClassDefinition *parentClass;
    // changeable attribute definitions
    // when checking inheritance, we traverse the chain
    // these should have public:, private:, or protected: at the beginning
    std::unordered_map<std::string, Attribute> attributes;
    // non-changeable methods
    // these cannot be changed, though an attribute can be a function
    std::unordered_map<std::string, Method> methods;
    // static versions
    std::unordered_map<std::string, Attribute> staticAttributes;
    std::unordered_map<std::string, Method> staticMethods;
};

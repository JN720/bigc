#include "ClassDefinition.h"
#include "FunctionNode.h"
#include "Object.h"

ClassDefinition::ClassDefinition()
{
}

ClassDefinition *ClassDefinition::getParent()
{
    return parentClass;
}

Result<Node *> ClassDefinition::getMethod(std::string name)
{
    if (methods.find(name) == methods.end())
        return Result<Node *>("undefined method");
    return Result<Node *>(methods[name]);
}

const std::unordered_set<Interface *> &ClassDefinition::getInterfaces()
{
    return interfaces;
}

void ClassDefinition::addMethod(std::string name, Node *method, bool isStatic)
{
    if (isStatic)
        staticMethods[name] = method;
    else
        methods[name] = method;
}

void ClassDefinition::addAttribute(std::string name, AccessSpecifier access, bool isStatic)
{
    if (isStatic)
        staticAttributeAccess[name] = access;
    else
        attributes[name] = access;
}

const std::unordered_map<std::string, AccessSpecifier> &ClassDefinition::getAttributes()
{
    return attributes;
}

const std::unordered_map<std::string, AccessSpecifier> &ClassDefinition::getStaticAttributeAccess()
{
    return staticAttributeAccess;
}

const std::unordered_map<std::string, Node *> &ClassDefinition::getMethods()
{
    return methods;
}

void ClassDefinition::applyInterface(Interface *interface)
{
    interfaces.insert(interface);
}

const std::unordered_map<std::string, Value> &ClassDefinition::getStaticAttributes()
{
    return staticAttributes;
}

const std::unordered_map<std::string, Node *> &ClassDefinition::getStaticMethods()
{
    return staticMethods;
}

void ClassDefinition::applyParent(ClassDefinition *parent)
{
    // attributes
    for (auto attribute : parent->getAttributes())
    {
        attributes.insert(attribute);
    }
    // static
    for (auto attribute : parent->getStaticAttributes())
    {
        staticAttributes.insert(attribute);
    }
    // interfaces
    for (auto interface : parent->getInterfaces())
    {
        interfaces.insert(interface);
    }
    // methods
    for (auto pair : parent->getMethods())
    {
        methods[pair.first] = pair.second;
    }
    // static
    for (auto pair : parent->getStaticMethods())
    {
        staticMethods[pair.first] = pair.second;
    }
}

bool ClassDefinition::implements(Interface *interface)
{
    return interfaces.find(interface) != interfaces.end();
}

Result<Node *> ClassDefinition::getClassMethod(std::string name)
{
    if (methods.find(name) != methods.end())
        return Result<Node *>(methods.at(name));
    return Result<Node *>("failed to find method '" + name + "'");
}

bool ClassDefinition::hasAttribute(std::string name)
{
    return attributes.find(name) != attributes.end();
}

Result<Value> ClassDefinition::construct(State *state, std::vector<Node *> &args)
{
    if (methods.find("constructor") == methods.end())
        return Result<Value>("failed to find constructor function");
    Node *methodNode = methods.at("constructor");
    if (FunctionNode *constructor = dynamic_cast<FunctionNode *>(methodNode))
    {
        Object *obj = new Object(this);
        for (auto attribute : attributeDefaults)
        {
            obj->addProperty(attribute.first, attribute.second);
        }
        Result<Value> result = constructor->executeInstanced(obj, state, args);
        if (!result.ok())
            return Result<Value>(result.getError());
        return Result<Value>(Value(obj));
    }
    return Result<Value>("constructor is not a function");
}

Result<Node *> ClassDefinition::getStaticMethod(std::string name)
{
    if (staticMethods.find(name) == staticMethods.end())
        return Result<Node *>("failed to find static method '" + name + "'");
    return Result<Node *>(staticMethods.at(name));
}

Result<Value> ClassDefinition::getStaticAttribute(std::string name)
{
    if (staticAttributes.find(name) == staticAttributes.end())
        return Result<Value>("failed to find static attribute '" + name + "'");
    return Result<Value>(Value(staticAttributes.at(name)));
}

void ClassDefinition::addStaticAttribute(std::string name, Value value, AccessSpecifier access)
{
    staticAttributes[name] = value;
    staticAttributeAccess[name] = access;
}

void ClassDefinition::setAttributeDefault(std::string name, Value value)
{
    attributeDefaults[name] = value;
}
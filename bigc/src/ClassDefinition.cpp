#include "ClassDefinition.h"

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
    {
        staticMethods[name] = method;
    }
    else
    {
        methods[name] = method;
    }
}

void ClassDefinition::addAttribute(std::string name, AccessSpecifier access, bool isStatic)
{
    if (isStatic)
    {
        staticAttributes[name] = access;
    }
    else
    {
        attributes[name] = access;
    }
}

const std::unordered_map<std::string, AccessSpecifier> &ClassDefinition::getAttributes()
{
    return attributes;
}

const std::unordered_map<std::string, Node *> &ClassDefinition::getMethods()
{
    return methods;
}

void ClassDefinition::applyInterface(Interface *interface)
{
    interfaces.insert(interface);
}

const std::unordered_map<std::string, AccessSpecifier> &ClassDefinition::getStaticAttributes()
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
    {
        return Result<Node *>(methods.at(name));
    }
    return Result<Node *>("failed to find method");
};
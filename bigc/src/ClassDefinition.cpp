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
    return Result<Node *>(methods.at(name).method);
}

const std::unordered_set<Interface *> &ClassDefinition::getInterfaces()
{
    return interfaces;
}

void ClassDefinition::addMethod(std::string name, Node *method, bool isStatic, AccessSpecifier access)
{
    if (isStatic)
        staticMethods[name] = Method(method, access);
    else
        methods[name] = Method(method, access);
}

void ClassDefinition::addAttribute(std::string name, AccessSpecifier access, bool isStatic, Value defaultValue)
{
    if (isStatic)
        staticAttributes[name] = Attribute(defaultValue, access, defaultValue);
    else
        attributes[name] = Attribute(defaultValue, access, defaultValue);
}

const std::unordered_map<std::string, Attribute> &ClassDefinition::getAttributes()
{
    return attributes;
}

const std::unordered_map<std::string, Attribute> &ClassDefinition::getAttributeDefaults()
{
    return attributes;
}

const std::unordered_map<std::string, Attribute> &ClassDefinition::getStaticAttributes()
{
    return staticAttributes;
}

const std::unordered_map<std::string, Method> &ClassDefinition::getMethods()
{
    return methods;
}

void ClassDefinition::applyInterface(Interface *interface)
{
    interfaces.insert(interface);
}

const std::unordered_map<std::string, Method> &ClassDefinition::getStaticMethods()
{
    return staticMethods;
}

void ClassDefinition::applyParent(ClassDefinition *parent)
{
    // attributes
    for (auto attribute : parent->getAttributes())
    {
        attributes[attribute.first] = attribute.second;
    }
    // static
    for (auto attribute : parent->getStaticAttributes())
    {
        staticAttributes[attribute.first] = attribute.second;
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
        return Result<Node *>(methods.at(name).method);
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
    Method constructor = methods.at("constructor");
    if (FunctionNode *methodNode = dynamic_cast<FunctionNode *>(constructor.method))
    {
        Object *obj = new Object(this);
        for (auto attribute : attributes)
        {
            obj->addProperty(attribute.first, attribute.second.defaultValue);
        }
        Result<Value> result = methodNode->executeInstanced(obj, state, args);
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
    return Result<Node *>(staticMethods.at(name).method);
}

Result<Value> ClassDefinition::getStaticAttribute(std::string name)
{
    if (staticAttributes.find(name) == staticAttributes.end())
        return Result<Value>("failed to find static attribute '" + name + "'");
    return Result<Value>(staticAttributes.at(name).value);
}

void ClassDefinition::setStaticAttribute(std::string name, Value value)
{
    staticAttributes[name].value = value;
}

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
    parentClass = parent;
    // attributes
    for (auto attribute : parent->getAttributes())
    {
        attributes[attribute.first] = attribute.second.inherit();
    }
    // static
    for (auto attribute : parent->getStaticAttributes())
    {
        staticAttributes[attribute.first] = attribute.second.inherit();
    }
    // interfaces
    for (auto interface : parent->getInterfaces())
    {
        interfaces.insert(interface);
    }
    // methods
    for (auto pair : parent->getMethods())
    {
        methods[pair.first] = pair.second.inherit();
    }
    // static
    for (auto pair : parent->getStaticMethods())
    {
        staticMethods[pair.first] = pair.second.inherit();
    }
}

bool ClassDefinition::canAccess(std::string property, AccessType instanced)
{
    if (instanced == THIS)
    {
        if (attributes.find(property) != attributes.end())
            return attributes.at(property).access != PARENT;
        if (methods.find(property) != methods.end())
            return methods.at(property).access != PARENT;
        if (staticMethods.find(property) != staticMethods.end())
            return staticMethods.at(property).access != PARENT;
        if (staticAttributes.find(property) != staticAttributes.end())
            return staticAttributes.at(property).access != PARENT;
        return false;
    }
    else if (instanced == SUPER)
    {
        if (attributes.find(property) != attributes.end())
            return attributes.at(property).access % 2 == 0;
        if (methods.find(property) != methods.end())
            return methods.at(property).access % 2 == 0;
        if (staticMethods.find(property) != staticMethods.end())
            return staticMethods.at(property).access % 2 == 0;
        if (staticAttributes.find(property) != staticAttributes.end())
            return staticAttributes.at(property).access % 2 == 0;
        return false;
    }
    if (staticAttributes.find(property) != staticAttributes.end())
        return staticAttributes.at(property).access == PUBLIC;
    if (staticMethods.find(property) != staticMethods.end())
        return staticMethods.at(property).access == PUBLIC;
    if (attributes.find(property) != attributes.end())
        return attributes.at(property).access == PUBLIC;
    if (methods.find(property) != methods.end())
        return methods.at(property).access == PUBLIC;
    return false;
}

bool ClassDefinition::implements(Interface *interface)
{
    return interfaces.find(interface) != interfaces.end();
}

// this returns a function node
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
            Value defaultVal = attribute.second.newDefault();
            if (Allocated *ref = state->getAllocated(defaultVal))
                state->addRef(ref);
            obj->addProperty(attribute.first, defaultVal);
        }
        if (!dynamic_cast<MethodNode *>(methodNode))
            methodNode = new MethodNode(methodNode, obj, *state);
        Result<Value> result = methodNode->execute(*state, args);
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

bool ClassDefinition::hasProperty(std::string name)
{
    if (attributes.find(name) != attributes.end())
        return true;
    if (methods.find(name) != methods.end())
        return true;
    if (staticAttributes.find(name) != staticAttributes.end())
        return true;
    if (staticMethods.find(name) != staticMethods.end())
        return true;
    return false;
}

ClassDefinition *ClassDefinition::getMethodClass(std::string name)
{
    Method method;
    bool isStatic = false;
    if (methods.find(name) != methods.end())
    {
        method = methods.at(name);
        isStatic = false;
    }
    else if (staticMethods.find(name) != staticMethods.end())
    {
        method = staticMethods.at(name);
        isStatic = true;
    }
    else
        return nullptr;
    if (method.access != PARENT)
        return this;
    ClassDefinition *currentParent = parentClass;
    // get the parent associated with the method
    while (currentParent)
    {
        if (isStatic)
        {
            if (currentParent->getStaticMethods().at(name).access == PRIVATE)
                return currentParent;
        }
        else
        {
            if (currentParent->getMethods().at(name).access == PRIVATE)
                return currentParent;
        }
        currentParent = currentParent->getParent();
    }
    return nullptr;
}

void ClassDefinition::destroy(State *state)
{
    // delete methods
    for (auto &method : methods)
    {
        state->removeRef(method.second.method);
    }

    // delete static methods
    for (auto &method : staticMethods)
    {
        state->removeRef(method.second.method);
    }
    // delete static attributes
    for (auto &attribute : staticAttributes)
    {
        if (Allocated *ref = state->getAllocated(attribute.second.defaultValue))
            state->removeRef(ref);
    }
    // delete static attributes
    for (auto &attribute : attributes)
    {
        if (Allocated *ref = state->getAllocated(attribute.second.defaultValue))
            state->removeRef(ref);
    }
    state->removeRef(parentClass);
    for (auto &interface : interfaces)
    {
        state->removeRef(interface);
    }
    state->removeRef(this);
}

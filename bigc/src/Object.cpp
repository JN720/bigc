#include "Object.h"

Object::Object()
{
}

Object::Object(ClassDefinitionInterface *definition)
{
    objClass = definition;
}

Object::Object(ClassDefinitionInterface *cls, std::unordered_map<std::string, Value> *attributes)
{
    objClass = cls;
    this->attributes = attributes;
}

ClassDefinitionInterface *Object::getClass()
{
    return objClass;
}

Control Object::setProperty(const std::string &property, const Value &value)
{
    if (!attributes)
        attributes = new std::unordered_map<std::string, Value>();
    if (attributes->find(property) == attributes->end())
        return Control("Property does not exist on object");
    (*attributes)[property] = value;
    return Control(OK);
}

void Object::addProperty(const std::string &property, const Value &value)
{
    if (!attributes)
        attributes = new std::unordered_map<std::string, Value>();
    (*attributes)[property] = value;
}

Result<Value> Object::getProperty(const std::string &property)
{
    if (!attributes)
        return Result<Value>("Object has no attributes");
    if (attributes->find(property) == attributes->end())
        return Result<Value>("Property does not exist on object");
    return Result<Value>((*attributes)[property]);
}

Object *Object::getSuper()
{
    return new Object(objClass->getParent(), attributes);
}

void Object::destroy(State *state)
{
    objClass->destroy(state);
}

Object::~Object()
{
    delete attributes;
}

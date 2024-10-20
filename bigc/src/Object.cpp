#include "Object.h"

Object::Object(ClassDefinitionInterface *definition)
{
    objClass = definition;
}

ClassDefinitionInterface *Object::getClass()
{
    return objClass;
}

Control Object::setProperty(const std::string &property, const Value &value)
{
    if (attributes.find(property) == attributes.end())
        return Control("Property does not exist on object");
    attributes[property] = value;
    return Control(OK);
}

void Object::addProperty(const std::string &property, const Value &value)
{
    attributes[property] = value;
}

Result<Value> Object::getProperty(const std::string &property)
{
    if (attributes.find(property) == attributes.end())
        return Result<Value>("Property does not exist on object");
    return Result<Value>(attributes[property]);
}

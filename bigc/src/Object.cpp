#include "Object.h"

Object::Object(ClassDefinition *definition)
{
    objClass = definition;
}

ClassDefinition *Object::getClass()
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

Result<Value> Object::getProperty(const std::string &property)
{
    if (attributes.find(property) == attributes.end())
        return Result<Value>("Property does not exist on object");
    return Result<Value>(attributes[property]);
}

#include "NullObject.h"

NullObject::NullObject()
{
}

Control NullObject::setProperty(const std::string &property, const Value &value)
{
    return Control("cannot set property on null object");
}

Result<Value> NullObject::getProperty(const std::string &property)
{
    return Result<Value>("cannot get property on null object");
}

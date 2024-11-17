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

NullObject *NullObject::getNull()
{
    if (!NullObject::nullObj)
        NullObject::nullObj = new NullObject();
    return NullObject::nullObj;
}

Object *NullObject::copy()
{
    return this;
}

Value NullObject::getDefaultFromType(const std::string &type)
{
    if (type == "bool")
        return Value(false);
    else if (type == "int")
        return Value(0);
    else if (type == "long")
        return Value(0L);
    else if (type == "float")
        return Value(0.0f);
    else if (type == "double")
        return Value(0.0);
    else if (type == "char")
        return Value('\0');
    else if (type == "str")
        return Value(new std::string(""));
    else if (type == "arr")
        return Value(new Array<Value>());
    return Value(NullObject::getNull());
}

NullObject *NullObject::nullObj = nullptr;

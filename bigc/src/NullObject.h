#pragma once

#include "Object.h"
#include "Array.h"

// disallows getting and setting properties
class NullObject : public Object
{
public:
    NullObject();
    Control setProperty(const std::string &property, const Value &value) override;
    Result<Value> getProperty(const std::string &property) override;
    static NullObject *getNull();
    Object *copy() override;
    static Value getDefaultFromType(const std::string &type);

private:
    static NullObject *nullObj;
};

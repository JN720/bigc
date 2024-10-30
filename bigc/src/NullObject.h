#pragma once

#include "Object.h"

// disallows getting and setting properties
class NullObject : public Object
{
public:
    NullObject();
    Control setProperty(const std::string &property, const Value &value) override;
    Result<Value> getProperty(const std::string &property) override;
};
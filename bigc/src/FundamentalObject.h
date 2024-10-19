#pragma once

#include "Object.h"

class FundamentalObject : public Object
{
public:
    Result<Value> getProperty(const std::string &property) override;
    Control setProperty(const std::string &property, const Value &value) override;
};
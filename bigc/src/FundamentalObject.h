#pragma once

#include "Object.h"

class FundamentalObject : public Object
{
public:
    FundamentalObject();
    virtual Result<Value> getProperty(const std::string &property) override;
    virtual Control setProperty(const std::string &property, const Value &value) override;
};
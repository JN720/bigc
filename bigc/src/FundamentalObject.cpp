#include "FundamentalObject.h"

FundamentalObject::FundamentalObject()
{
}

Result<Value> FundamentalObject::getProperty(const std::string &property)
{
    return Result<Value>("FundamentalObject cannot have properties get");
}

Control FundamentalObject::setProperty(const std::string &property, const Value &value)
{
    return Control("FundamentalObject cannot have properties set");
}
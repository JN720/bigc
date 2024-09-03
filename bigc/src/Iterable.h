#pragma once
#include "Value.h"
#include "Result.h"

class Iterable
{
    virtual std::string add(Value val);
    virtual Result<> get(Value index);
    virtual Result<> len();
};
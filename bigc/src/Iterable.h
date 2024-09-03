#pragma once
#include "Value.h"
#include "Result.h"

class Iterable
{
public:
    virtual std::string add(Value val);
    virtual Result<Value> get(Value index);
    virtual Result<Value> len();
};
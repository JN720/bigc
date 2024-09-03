#pragma once
#include "Value.h"
#include "Iterable.h"
#include <vector>

class Array : public Iterable
{
public:
    std::string add(Value val) override;
    Result<> get(Value index) override;
    Result<> len() override;
    Array();

protected:
    std::vector<Value> values;
};
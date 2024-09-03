#pragma once
#include "Iterable.h"
#include <vector>

class Array : public Iterable
{
public:
    Array();
    std::string add(Value val) override;
    Result<Value> get(Value index) override;
    Result<Value> len() override;

protected:
    std::vector<Value> values;
};
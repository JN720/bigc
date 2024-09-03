#include "Array.h"

std::string Array::add(Value val)
{
    values.push_back(val);
}

Result<> Array::get(Value index)
{
    if (index.getType() != "int")
        return Result<>("index expression for array must be int");
    return Result<>(values[*(int *)index.getValue()]);
}

Result<> Array::len()
{
    return Result<>(Value("int", (void *)values.size()));
}

Array::Array()
{
}

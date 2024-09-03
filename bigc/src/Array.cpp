#include "Array.h"

std::string Array::add(Value val)
{
    values.push_back(val);
    return "";
}

Result<Value> Array::get(Value index)
{
    Wildcard x;
    if (auto i = std::get_if<int *>(&x))
        return Result<Value>(values[**i]);
    else if (auto i = std::get_if<long *>(&x))
        return Result<Value>(values[**i]);
    return Result<Value>("index expression for array must be integeric");
}

Result<Value> Array::len()
{
    return Result<Value>(Value(new int(values.size())));
}

Array::Array()
{
}

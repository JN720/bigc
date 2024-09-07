#pragma once
#include "Value.h"
#include <vector>

template <class T>
class Array : public Iterable<T>
{
public:
    Array<T>();
    std::string add(Value val) override;
    Result<T> get(Hashable index) override;
    Result<T> len() override;

protected:
    std::vector<T> values;
};

template <class T>
std::string Array<T>::add(Value val)
{
    values.push_back(val);
    return "";
}

template <class T>
Result<T> Array<T>::get(Hashable index)
{
    Wildcard x;
    auto result = Result<Value>();
    if (auto i = std::get_if<int *>(&x))
        return Result<Value>(values[**i]);
    else if (auto i = std::get_if<long *>(&x))
        return Result<Value>(values[**i]);
    result.setError("index expression for array must be integeric");
    return result;
}

template <class T>
Result<T> Array<T>::len()
{
    return Result<Value>(Value(new int(values.size())));
}

template <class T>
Array<T>::Array()
{
}
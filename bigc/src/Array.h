#pragma once
#include "Value.h"
#include <vector>

template <class T>
class Array : public Iterable<T>
{
public:
    Array<T>();
    std::string add(Value val) override;
    Result<T> get(Hashable *index) override;
    Result<int> len() override;

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
Result<T> Array<T>::get(Hashable *index)
{
    Result<int> result = index->hash();
    if (!result.ok())
        return "while hashing:\n" + result.getError();
    int location = result.getValue();
    Result<int> length = len();
    if (!length.ok())
        return "while getting length:\n" + length.getError();
    int l = length.getValue();
    if (location >= l)
        return Result<T>("index out of bounds");
    return Result<Value>(values[location]);
}

template <class T>
Result<int> Array<T>::len()
{
    return Result<int>(values.size());
}

template <class T>
Array<T>::Array()
{
}
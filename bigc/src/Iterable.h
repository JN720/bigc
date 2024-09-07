#pragma once
#include "Hashable.h"
#include <vector>

template <class T>
class Iterable
{
public:
    virtual std::string add(T val);
    virtual Result<T> get(Hashable index);
    virtual Result<T> len();
    virtual Result<Iterable<T> *> combine(Iterable<T> &other);
};

template <class T>
std::string Iterable<T>::add(T val)
{
    return std::string();
}

template <class T>
Result<T> Iterable<T>::get(Hashable index)
{
    auto result = Result<T>();
    result.setError("not implemented");
    return result;
}

template <class T>
Result<T> Iterable<T>::len()
{
    auto result = Result<T>();
    result.setError("not implemented");
    return result;
}

template <class T>
Result<Iterable<T> *> Iterable<T>::combine(Iterable<T> &other)
{
    return Result<Iterable *>("not implemented");
}
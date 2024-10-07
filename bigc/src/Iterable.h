#pragma once
#include "Hashable.h"
#include <vector>

template <class T>
class Iterable
{
public:
    virtual std::string add(T val);
    virtual Result<T> get(Hashable *index);
    virtual Result<int> len();
    virtual Result<Iterable<T> *> combine(Iterable<T> &other);
    virtual Control set(Hashable *index, T value);
};

template <class T>
inline std::string Iterable<T>::add(T val)
{
    return std::string();
}

template <class T>
inline Result<T> Iterable<T>::get(Hashable *index)
{
    auto result = Result<T>();
    result.setError("not implemented");
    return result;
}

template <class T>
inline Result<int> Iterable<T>::len()
{
    return Result<int>("not implemented");
}

template <class T>
inline Result<Iterable<T> *> Iterable<T>::combine(Iterable<T> &other)
{
    return Result<Iterable *>("not implemented");
}

template <class T>
inline Control Iterable<T>::set(Hashable *index, T value)
{
    return Control("not implemented");
}

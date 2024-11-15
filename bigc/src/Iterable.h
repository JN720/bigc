#pragma once
#include "Hashable.h"
#include "Result.h"
#include "Allocated.h"
#include <vector>

template <class T>
struct IteratorResult
{
    T value;
    int nextState;
};

template <class T>
class Iterable : public Allocated
{
public:
    virtual Control add(T val);
    virtual Result<T> get(Hashable *index);
    virtual Result<int> len();
    virtual Result<Iterable<T> *> combine(Iterable<T> &other);
    virtual Control set(Hashable *index, T value);
    virtual Result<IteratorResult<T>> next(int state);
};

template <class T>
inline Control Iterable<T>::add(T val)
{
    return Control("not implemented");
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

template <class T>
inline Result<IteratorResult<T>> Iterable<T>::next(int state)
{
    return Result<IteratorResult<T>>(Control("not implemented"));
}

#pragma once
#include "Value.h"
#include "Iterable.h"
#include "State.h"
#include <vector>

template <class T>
class Array : public Iterable<T>
{
public:
    Array<T>();
    Control add(Value val) override;
    Result<T> get(Hashable *index) override;
    Result<int> len() override;
    Result<IteratorResult<T>> next(int state) override;
    void destroy(State *state) override;

protected:
    std::vector<T> values;
};

template <class T>
Control Array<T>::add(Value val)
{
    values.push_back(val);
    return Control(OK);
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
    std::vector<T> values;
}

template <class T>
Result<IteratorResult<T>> Array<T>::next(int state)
{
    if (state >= values.size())
    {
        return Result<IteratorResult<T>>(Control(BREAK));
    }
    return Result<IteratorResult<T>>(IteratorResult<T>{values[state], state + 1});
}

template <class T>
void Array<T>::destroy(State *state)
{
    for (T &value : values)
    {
        if (Value *val = dynamic_cast<Value *>(&value))
        {
            if (Allocated *ref = state->getAllocated(*val))
                state->removeRef(ref);
        }
    }
    state->removeRef(this);
}
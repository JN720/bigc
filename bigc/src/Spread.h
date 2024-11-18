#pragma once
#include "Array.h"

template <class T>
class Spread : public Array<T>
{
public:
    Control applyIterable(Iterable<T> *iterable);
    typename std::vector<T>::iterator begin() { return this->values.begin(); }
    typename std::vector<T>::iterator end() { return this->values.end(); }
    typename std::vector<T>::const_iterator begin() const { return this->values.begin(); }
    typename std::vector<T>::const_iterator end() const { return this->values.end(); }
    T at(int index);
};

template <class T>
inline Control Spread<T>::applyIterable(Iterable<T> *iterable)
{
    int state = 0;
    while (true)
    {
        Result<IteratorResult<T>> result = iterable->next(state);
        if (!result.ok())
        {
            if (result.getSignal() == BREAK)
                break;
            else
                return Control(result.getError()).stack("during spread:\n");
        }
        state = result.getValue().nextState;
        this->values.push_back(result.getValue().value);
    }
    return Control(OK);
}

template <class T>
inline T Spread<T>::at(int index)
{
    return this->values[index];
}

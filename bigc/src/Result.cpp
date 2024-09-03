#include "Result.h"

template <class T>
Result<T>::Result(T val)
{
    value = val;
    error = "";
}

template <class T>
Result<T>::Result(std::string str)
{

    error = str;
}

template <class T>
bool Result<T>::ok()
{
    return error.empty();
}

template <class T>
T Result<T>::getValue()
{
    if (!ok())
        throw error;
    return value;
}

template <class T>
std::string Result<T>::getError()
{
    return error;
}
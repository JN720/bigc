#pragma once
#include <iostream>
#include <variant>
#include "Control.h"

template <class T>
using ValErr = std::variant<T, std::string>;

template <class T>
class Result
{
public:
    Result<T>();
    // when the result is ok
    Result<T>(T val);

    // when there is an error
    Result<T>(std::string str);

    // valueless signal (exit)
    Result<T>(Signal s);

    // valued signal (return, break, continue)
    Result<T>(Signal s, T val);

    // from a control
    Result<T>(Control control);

    Result<T> stack(std::string err);
    bool ok();
    T getValue();
    std::string getError();
    void setError(std::string error);
    Signal getSignal();
    // this is because of unions I think
    // Result<T> operator=(const Result<T> &other);

private:
    ValErr<T> value;
    Signal signal;
};

template <class T>
inline Result<T>::Result()
{
    value = std::string("unknown error");
    signal = ERROR;
}

template <class T>
inline Result<T>::Result(T val)
{
    value = val;
    signal = OK;
}

template <class T>
inline Result<T>::Result(std::string str)
{
    value = str;
    signal = ERROR;
}

template <class T>
inline Result<T>::Result(Signal s)
{
    signal = s;
}

template <class T>
inline Result<T>::Result(Signal s, T val)
{
    signal = s;
    value = val;
}

template <class T>
inline bool Result<T>::ok()
{
    return signal == OK;
}

template <class T>
inline T Result<T>::getValue()
{
    if (T *x = std::get_if<T>(&value))
        return *x;
    std::cout << "nope you can't access the value in the result!\n";
    throw "value does not exist";
}

template <class T>
inline std::string Result<T>::getError()
{
    if (std::string *x = std::get_if<std::string>(&value))
        return *x;
    std::cout << "nope you can't access the error in the result!\n";
    throw "no error";
}

template <class T>
inline void Result<T>::setError(std::string error)
{
    value = error;
}

template <class T>
inline Signal Result<T>::getSignal()
{
    return signal;
}

template <class T>
inline Result<T>::Result(Control control)
{
    value = control.getError();
    signal = control.getSignal();
}

template <class T>
inline Result<T> Result<T>::stack(std::string err)
{
    if (std::string *x = std::get_if<std::string>(&value))
        value = err + *x;
    else
        throw "error does not exist";
    return *this;
}

/*
template <class T>
inline Result<T> Result<T>::operator=(const Result<T> &other)
{
    signal = other.signal;
    value = other.value;
    return *this;
}
*/
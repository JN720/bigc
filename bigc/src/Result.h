#pragma once
#include "Value.h"
#include <iostream>

template <class T>
class Result
{
public:
    Result<T>(T val);
    Result<T>(std::string str);
    bool ok();
    T getValue();
    std::string getError();

private:
    T value;
    std::string error;
};

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
    {
        std::cout << "nope you can't access the value in the result!\n";
        throw error;
    }
    return value;
}

template <class T>
std::string Result<T>::getError()
{
    return error;
}
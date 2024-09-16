#pragma once
#include <iostream>

template <class T>
class Result
{
public:
    Result<T>();
    Result<T>(T val);
    Result<T>(std::string str);
    bool ok();
    T getValue();
    std::string getError();
    void setError(std::string error);

private:
    T value;
    std::string error;
};

template <class T>
Result<T>::Result()
{
    value = T();
    error = "";
}

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
        return value;
    }
    return value;
}

template <class T>
std::string Result<T>::getError()
{
    return error;
}

template <class T>
void Result<T>::setError(std::string error)
{
    this->error = error;
}
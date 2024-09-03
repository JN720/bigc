#pragma once
#include <string>
#include "Value.h"

template <class T = Value>
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

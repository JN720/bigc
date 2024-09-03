#pragma once
#include <string>
#include "Token.h"
#include <variant>
#include "Result.h"

class Array;
class Node;
class Object;

/*
how type signatures will work:
fundamental types: int, str, float, double, char
arrays: arr for dynamic and arr:int for typed arrays
functions:
    fully dynamic: fn
    with return: fn:int
    with args: fn(int)
    args and return: fn:int(int)
    with tempalted types: fn:arr:int(arr:int, fn:int())
    with spread: fn:void:(~int)

*/

using Wildcard = std::variant<int *, long *, float *, double *, char *, std::string *, Array *, Node *, Object *>;

class Value
{
public:
    Value();
    // infer value from token
    Value(Token &token);
    Value(Wildcard value);
    Wildcard getValue();
    std::string getType();
    Result<Value> add(Value other);
    Result<Value> negate();
    Result<Value> subtract(Value other);
    Result<Value> multiply(Value other);
    Result<Value> reciprocate();
    Result<Value> divide(Value other);

protected:
    Wildcard value;
};

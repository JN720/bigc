#pragma once
#include <string>
#include <unordered_map>
#include "Token.h"
#include <variant>
#include "Iterable.h"

class Node;
class Object;

/*
how type signatures will work:
fundamental types: bool, int, str, float, double, char
arrays: arr for dynamic and arr:int for typed arrays
functions:
    fully dynamic: fn
    with return: fn:int
    with args: fn(int)
    args and return: fn:int(int)
    with tempalted types: fn:arr:int(arr:int, fn:int())
    with spread: fn:void:(~int)

*/
class Value;

using Wildcard = std::variant<bool, int, long, float, double, char, std::string *, Iterable<Value> *, Node *, Object *>;

class Value : public Hashable
{
public:
    Value();
    // infer value from token
    Value(Token &token);
    Value(Wildcard value);
    Result<int> hash() override;
    Wildcard getValue() const;
    std::string getType() const;
    Result<Value> add(Value other);
    Result<Value> inverse();
    Result<Value> subtract(Value other);
    Result<Value> multiply(Value other);
    Result<Value> reciprocate();
    Result<Value> divide(Value other);
    Result<Value> modulo(Value other);
    Result<Value> isEqual(Value other);
    Result<Value> isNotEqual(Value other);
    Result<Value> isLessThan(Value other);
    Result<Value> isGreaterThan(Value other);
    Result<Value> isLessThanEqual(Value other);
    Result<Value> isGreaterThanEqual(Value other);
    Result<Value> negate();

    Allocated *getAllocated();

protected:
    Wildcard value;
};

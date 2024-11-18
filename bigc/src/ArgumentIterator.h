#pragma once

#include "SpreadNode.h"
#include <vector>

class Value; // Forward declaration of Value

class ArgumentIterator
{
public:
    ArgumentIterator(std::vector<Node *> &args);
    Result<Value> next(State *state);

private:
    std::vector<Node *> args;
    size_t index;
    size_t spreadIndex;
};
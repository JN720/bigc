#pragma once
#include "Node.h"

namespace base
{

    std::string fmtValue(Wildcard val);

    void printValue(Wildcard val);

    Result<Value> print(State &state, std::vector<Node *> &args);

    Result<Value> println(State &state, std::vector<Node *> &args);

    Result<Value> len(State &state, std::vector<Node *> &args);

    Result<Value> input(State &state, std::vector<Node *> &args);

    // imported groups
    Result<Value> library(State &state, std::vector<Node *> &args);
    // built-in/installed libraries
    Result<Value> include(State &state, std::vector<Node *> &args);
    // imported files
    Result<Value> import(State &state, std::vector<Node *> &args);

    void debugPrint(Value value);

    Result<Value> executeFundamentalFunction(int index, State &state, std::vector<Node *> &args);
};

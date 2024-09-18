#pragma once
#include "Node.h"

namespace base
{

    void printValue(Wildcard val);

    Result<Value> print(State &state, std::vector<Node *> &args);

    void println(State &state, std::vector<Node *> &args);

    Result<Value> len(State &state, std::vector<Node *> &args);

    Result<Value> input(State &state, std::vector<Node *> &args);

    void debugPrint(Value value);

    Result<Value> executeFundamentalFunction(int index, State &state, std::vector<Node *> &args);
};

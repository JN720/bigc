#pragma once
#include "VariableNode.h"
#include "Iterable.h"

/*
    IterateNode is used to iterate over an iterable.
    iteration looks like this:
    iterate var iterable {
        ...
    };
*/
class IterateNode : public VariableNode
{
public:
    IterateNode();
    IterateNode(std::string var);
    Control resolve(State &state) override;

private:
    Iterable<Value> *iterator;
    int currentState;
};

#pragma once
#include "Node.h"

/*
This is functionally a wrapper node but will be
recognized by sequence nodes
*/
class SignalNode : public Node
{
public:
    SignalNode(Signal s);
    Value getValue(const State &state);
    std::string resolve(State &state);

private:
    Signal signal;
};

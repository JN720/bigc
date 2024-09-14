#pragma once
#include "Node.h"

/*
This is functionally a wrapper node but will be
recognized by sequence nodes
*/
class ReturnNode : public Node
{
public:
    ReturnNode();
    Value getValue(const State &state);
    std::string resolve(State &state);
};

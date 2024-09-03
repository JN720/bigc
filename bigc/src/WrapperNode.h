#pragma once
#include "Node.h"

class WrapperNode : public Node
{
public:
    WrapperNode();
    Value getValue(const State &state);
    std::string resolve(State &state);
};

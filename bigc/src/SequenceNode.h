#pragma once
#include "Node.h"

// children are statement results
class SequenceNode : public Node
{
public:
    SequenceNode();
    Value getValue(const State &state);
    std::string resolve(State &state) override;
};
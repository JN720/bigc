#pragma once
#include "Node.h"

// children are statement results
class SequenceNode : public Node
{
public:
    SequenceNode();
    std::string resolve(State &state) override;
};
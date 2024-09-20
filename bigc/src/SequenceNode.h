#pragma once
#include "Node.h"

// children are statement results
class SequenceNode : public Node
{
public:
    SequenceNode();
    Control resolve(State &state) override;
};
#pragma once
#include "Node.h"

// first child is true, second is false
class LoopNode : public Node
{
public:
    LoopNode();
    Control resolve(State &state) override;
};
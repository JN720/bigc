#pragma once
#include "Node.h"

// first child is true, second is false
class TryNode : public Node
{
public:
    TryNode();
    Control resolve(State &state) override;
};
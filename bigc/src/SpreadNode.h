#pragma once
#include "Node.h"
class SpreadNode : public Node
{
public:
    SpreadNode();
    Control resolve(State &state) override;
};

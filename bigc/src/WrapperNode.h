#pragma once
#include "Node.h"

class WrapperNode : public Node
{
public:
    WrapperNode();
    Value getValue(const State &state);
    Control resolve(State &state);
    Node *copy() override;
};

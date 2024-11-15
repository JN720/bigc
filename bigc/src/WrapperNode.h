#pragma once
#include "Node.h"

class WrapperNode : public Node
{
public:
    WrapperNode();
    Value getValue(const State &state) override;
    Control resolve(State &state) override;
    Node *copy() override;
};

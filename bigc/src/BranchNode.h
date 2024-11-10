#pragma once
#include "Node.h"

// first child is true, second is false
class BranchNode : public Node
{
public:
    BranchNode();
    Control resolve(State &state) override;
    Node *copy() override;
};
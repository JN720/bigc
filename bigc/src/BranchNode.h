#pragma once
#include "Node.h"

// first child is true, second is false
class BranchNode : public Node
{
public:
    BranchNode();
    std::string resolve(State &state) override;
};
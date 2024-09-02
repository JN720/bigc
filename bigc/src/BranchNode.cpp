#include "BranchNode.h"

BranchNode::BranchNode()
{
    type = N_BRANCH;
}

Value BranchNode::getValue(const State &state)
{
    return Value();
}

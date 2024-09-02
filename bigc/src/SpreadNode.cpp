#include "SpreadNode.h"

SpreadNode::SpreadNode()
{
    type = N_SPREAD;
}

Value SpreadNode::getValue(const State &state)
{
    return Value();
}

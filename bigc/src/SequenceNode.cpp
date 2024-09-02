#include "SequenceNode.h"

SequenceNode::SequenceNode()
{
    type = N_SEQUENCE;
}

Value SequenceNode::getValue(const State &state)
{
    return Value();
}

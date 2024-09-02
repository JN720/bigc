#include "PipeNode.h"

PipeNode::PipeNode()
{
    type = N_PIPE;
}

Value PipeNode::getValue(const State &state)
{
    return Value();
}

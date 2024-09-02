#include "WrapperNode.h"

WrapperNode::WrapperNode()
{
    type = N_WRAPPER;
}

Value WrapperNode::getValue(const State &state)
{
    return Value();
}
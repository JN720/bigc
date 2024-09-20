#include "WrapperNode.h"

WrapperNode::WrapperNode()
{
    type = N_WRAPPER;
}

Value WrapperNode::getValue(const State &state)
{
    return children[0]->getValue(state);
}

Control WrapperNode::resolve(State &state)
{
    if (children.empty())
        return Control("empty wrapper");
    return children[0]->resolve(state);
}

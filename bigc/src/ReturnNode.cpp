#include "ReturnNode.h"

ReturnNode::ReturnNode()
{
    type = N_RETURN;
}

Value ReturnNode::getValue(const State &state)
{
    return children[0]->getValue(state);
}

std::string ReturnNode::resolve(State &state)
{
    if (children.empty())
        return "empty wrapper";
    return children[0]->resolve(state);
}

#include "UnbranchNode.h"

UnbranchNode::UnbranchNode()
{
    type = N_UNBRANCH;
}

std::string UnbranchNode::resolve(State &state)
{
    for (auto child : children)
    {
        std::string error = child->resolve(state);
        if (!error.empty())
            return error;
    }
    value = children.back()->getValue(state);
    return "";
}

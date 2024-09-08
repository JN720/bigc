#include "SequenceNode.h"

SequenceNode::SequenceNode()
{
    type = N_SEQUENCE;
}

std::string SequenceNode::resolve(State &state)
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
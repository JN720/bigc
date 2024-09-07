#include "SequenceNode.h"

SequenceNode::SequenceNode()
{
    type = N_SEQUENCE;
}

Value SequenceNode::getValue(const State &state)
{
    return Value();
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
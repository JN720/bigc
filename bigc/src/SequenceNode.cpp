#include "SequenceNode.h"

SequenceNode::SequenceNode()
{
    type = N_SEQUENCE;
}

std::string SequenceNode::resolve(State &state)
{
    for (int i = 0; i < children.size(); i++)
    {
        std::string error = children[i]->resolve(state);
        if (!error.empty())
            return "in statement " + std::to_string(i + 1) + " of sequence:\n" + error;
    }
    if (children.size())
        value = children.back()->getValue(state);
    return "";
}
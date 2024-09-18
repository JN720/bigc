#include "SignalNode.h"

SignalNode::SignalNode(Signal s)
{
    signal = s;
    type = N_SIGNAL;
}

Value SignalNode::getValue(const State &state)
{
    return children[0]->getValue(state);
}

std::string SignalNode::resolve(State &state)
{
    if (children.empty())
        return "empty wrapper";
    return children[0]->resolve(state);
}

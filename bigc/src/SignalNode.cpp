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

Control SignalNode::resolve(State &state)
{
    // null
    if (children.empty())
    {
        return Control("empty wrapper");
    }
    return children[0]->resolve(state);
}
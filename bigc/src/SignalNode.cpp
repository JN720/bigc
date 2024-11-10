#include "SignalNode.h"

SignalNode::SignalNode(Signal s)
{
    signal = s;
    type = N_SIGNAL;
}

Value SignalNode::getValue(const State &state)
{
    if (hasChildren())
        return children[0]->getValue(state);
    return Value();
}

Control SignalNode::resolve(State &state)
{
    // this can be void
    if (!hasChildren())
        return Control(signal);
    Control control = children[0]->resolve(state);
    if (control.control())
    {
        value = children[0]->getValue(state);
        return control;
    }
    if (control.error())
        return control.stack("while evaluating signal:\n");
    return signal;
}

Node *SignalNode::copy()
{
    SignalNode *sig = new SignalNode(signal);
    if (children.size())
        sig->addChild(children[0]->copy());
    return sig;
}

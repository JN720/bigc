#include "SequenceNode.h"
#include "SignalNode.h"

SequenceNode::SequenceNode()
{
    type = N_SEQUENCE;
}

Control SequenceNode::resolve(State &state)
{
    for (int i = 0; i < children.size(); i++)
    {
        Control control = children[i]->resolve(state);
        if (control.control())
        {
            if (children[i]->hasChildren())
                value = children[i]->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("in statement " + std::to_string(i + 1) + " of sequence:\n");
    }
    if (children.size())
        value = children.back()->getValue(state);
    return Control(OK);
}
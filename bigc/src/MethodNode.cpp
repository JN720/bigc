#include "MethodNode.h"

MethodNode::MethodNode()
{
    type = N_METHOD;
}

Control MethodNode::resolve(State &state)
{
    if (children.size() != 1)
    {
        return Control("method declaration should only have 1 function");
    }
    Control control = children[0]->resolve(state);
    if (control.control())
    {
        value = children[0]->getValue(state);
        return control;
    }
    if (control.error())
        return control.stack("during method declaration:\n");

    value = children[0]->getValue(state);
    return Control(OK);
}

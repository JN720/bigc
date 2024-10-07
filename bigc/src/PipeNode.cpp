#include "PipeNode.h"
#include "IdentifierNode.h"
#include "CallNode.h"

PipeNode::PipeNode()
{
    type = N_PIPE;
}

Control PipeNode::resolve(State &state)
{
    // resolve left side of pipe
    Control control = children[0]->resolve(state);
    if (control.control())
    {
        value = children[0]->getValue(state);
        return control;
    }
    if (control.error())
        return control.stack("resolving pipe value:\n");
    // add pipe to new state frame
    StateFrame *frame = state.pushFrame();
    frame->setVariable("&", children[0]->getValue(state));
    // if it's just a single identifier,
    // pass in the pipe value as the first argument
    if (children[1]->getType() == N_IDENTIFIER)
    {
        IdentifierNode *identifier = (IdentifierNode *)children[1];
        CallNode *call = new CallNode(identifier);
        call->addChild(new IdentifierNode());
        control = identifier->resolve(state);
        if (control.control())
        {
            value = identifier->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("resolving pipe function:\n");
        value = identifier->getValue(state);
    }
    // if we pipe into an expression
    else
    {
        control = children[1]->resolve(state);
        if (control.control())
        {
            value = children[1]->getValue(state);
        }
        if (control.error())
            return control.stack("resolving pipe expression:\n");
        value = children[1]->getValue(state);
    }
    return Control(OK);
}

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
    StateFrame *frame = state.pushFrame(false);
    frame->setVariable("&", children[0]->getValue(state));
    // if it's a variable node
    // pass in the pipe value as the first argument
    if (VariableNode *var = dynamic_cast<VariableNode *>(children[1]))
    {
        CallNode *call = new CallNode(var);
        call->addChild(new IdentifierNode());
        control = call->resolve(state);
        if (control.control())
        {
            state.popFrame();
            value = call->getValue(state);
            return control;
        }
        if (control.error())
        {
            state.popFrame();
            return control.stack("resolving pipe function:\n");
        }
        value = var->getValue(state);
    }
    // if we pipe into an expression
    else
    {
        control = children[1]->resolve(state);
        if (control.control())
        {
            state.popFrame();
            value = children[1]->getValue(state);
            return control;
        }
        if (control.error())
        {
            state.popFrame();
            return control.stack("resolving pipe expression:\n");
        }
        value = children[1]->getValue(state);
    }
    return Control(OK);
}

#include "PipeNode.h"
#include "IdentifierNode.h"

PipeNode::PipeNode()
{
    type = N_PIPE;
}

std::string PipeNode::resolve(State &state)
{
    // resolve left side of pipe
    std::string error = children[0]->resolve(state);
    if (!error.empty())
        return "resolving pipe value:\n" + error;
    // add pipe to new state frame
    StateFrame *frame = state.pushFrame();
    frame->setVariable("&", children[0]->getValue(state));
    // if it's just a single identifier,
    // pass in the pipe value as the first argument
    if (children[1]->getType() == N_IDENTIFIER)
    {
        IdentifierNode *identifier = (IdentifierNode *)children[1];
        identifier->addChild(new IdentifierNode());
        identifier->makeCall();
        error = identifier->resolve(state);
        if (!error.empty())
            return "resolving pipe function:\n" + error;
        value = identifier->getValue(state);
    }
    // if we pipe into an expression
    else
    {
        error = children[1]->resolve(state);
        if (!error.empty())
            return "resolving pipe expression:\n" + error;
        value = children[1]->getValue(state);
    }
    return "";
}

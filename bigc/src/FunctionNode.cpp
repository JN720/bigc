#include "FunctionNode.h"

FunctionNode::FunctionNode()
{
    type = N_LEAFVALUE;
}

std::string FunctionNode::resolve(State &state)
{
    if (children.size() < 1)
        return "function must have sequence";
    return "failed to execute function";
}

Result<Value> execute(State &state, std::vector<Node *> &args)
{
}
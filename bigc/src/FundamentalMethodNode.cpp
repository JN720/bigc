#include "FundamentalMethodNode.h"

Result<Value> FundamentalMethodNode::execute(State &state, std::vector<Node *> &args)
{
    return Result<Value>("FundamentalMethodNode cannot be executed without an instance");
}

Control FundamentalMethodNode::resolve(State &state)
{
    value = Value(this);
    return Control(OK);
}

#include "SocketFundamentalMethodNode.h"
#include "../../FunctionNode.h"

SocketFundamentalMethodNode::SocketFundamentalMethodNode()
{
    method = [](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
    {
        return Result<Value>("SocketFundamentalMethodNode cannot be executed without a function");
    };
}

SocketFundamentalMethodNode::SocketFundamentalMethodNode(SocketMethodFunction func)
{
    method = func;
}

Result<Value> SocketFundamentalMethodNode::executeInstanced(Object *obj, State *state, std::vector<Node *> &args)
{
    // Control control = FunctionNode::resolveArguments(state, args);
    // if (!control.ok())
    //     return Result<Value>(control);
    SocketFundamentalObject *socketObj = dynamic_cast<SocketFundamentalObject *>(obj);
    if (!socketObj)
        return Result<Value>("object is not a socket");
    return this->method(socketObj, *state, args);
}

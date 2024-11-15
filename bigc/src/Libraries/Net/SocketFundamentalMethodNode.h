#pragma once

#include <functional>
#include "../../FundamentalMethodNode.h"
#include "SocketFundamentalClass.h"

class SocketFundamentalMethodNode : public FunctionNode
{
public:
    using SocketMethodFunction = std::function<Result<Value>(SocketFundamentalObject *, State &, std::vector<Node *> &)>;
    SocketFundamentalMethodNode();
    SocketFundamentalMethodNode(SocketMethodFunction func);
    Result<Value> executeInstanced(Object *obj, State *state, std::vector<Node *> &args) override;

private:
    SocketMethodFunction method;
};
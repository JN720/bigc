#pragma once

#include "FunctionNode.h"
#include "builtin.h"

class FundamentalFunctionNode : public FunctionNode
{
private:
    int functionIndex;

public:
    FundamentalFunctionNode(const std::string &name);
    Result<Value> execute(State &state, std::vector<Node *> &args) override;
    Node *copy() override;
};

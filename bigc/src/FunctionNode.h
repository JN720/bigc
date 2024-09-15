#pragma once
#include "Node.h"

class FunctionNode : public Node
{
public:
    FunctionNode();
    virtual std::string resolve(State &state) override;
    Result<Value> execute(State &state, std::vector<Node *> &args);
};
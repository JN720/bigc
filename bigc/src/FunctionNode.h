#pragma once
#include "Node.h"

class FunctionNode : public Node
{
public:
    FunctionNode();
    Control resolve(State &state) override;
    virtual Result<Value> execute(State &state, std::vector<Node *> &args);
    Result<Value> executeInstanced(Object *obj, State *state, std::vector<Node *> &args);
};
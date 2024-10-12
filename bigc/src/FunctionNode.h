#pragma once
#include "Node.h"

// the arguments are first then the sequence
class FunctionNode : public Node
{
public:
    FunctionNode();
    Control resolve(State &state) override;
    virtual Result<Value> execute(State &state, std::vector<Node *> &args);
    Result<Value> executeInstanced(Object *obj, State *state, std::vector<Node *> &args);
};
#pragma once

#include "FunctionNode.h"

class MethodNode : public FunctionNode
{
public:
    MethodNode(Node *method, Object *object);
    Result<Value> execute(State &state, std::vector<Node *> &args) override;

private:
    Object *object;
};

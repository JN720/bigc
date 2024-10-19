#pragma once
#include "MethodNode.h"

class FundamentalMethodNode : public MethodNode
{
public:
    FundamentalMethodNode(Node *method, Object *object);
    Result<Value> execute(State &state, std::vector<Node *> &args) override;
    Control resolve(State &state) override;
};
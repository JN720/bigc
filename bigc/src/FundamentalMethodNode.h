#pragma once
#include "MethodNode.h"

// this is the superclass for all fundamental methods
// each fundamental method must implement the execute method
class FundamentalMethodNode : public MethodNode
{
public:
    FundamentalMethodNode();
    FundamentalMethodNode(Node *method, Object *object);
    Result<Value> execute(State &state, std::vector<Node *> &args) override;
    Control resolve(State &state) override;
};
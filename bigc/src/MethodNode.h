#pragma once
#include "VariableNode.h"

// this contains a variable and encapsulates a function node
class MethodNode : public VariableNode
{
public:
    MethodNode();
    Control resolve(State &state) override;
};
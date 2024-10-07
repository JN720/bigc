#pragma once
#include "VariableNode.h"

class IndexNode : public VariableNode
{
public:
    IndexNode();
    Control resolve(State &state) override;
    Control setValue(State &state, Value value) override;
};

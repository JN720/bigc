#pragma once
#include "VariableNode.h"

class AccessNode : public VariableNode
{
private:
    std::string property;

public:
    AccessNode(const std::string &name);
    Control resolve(State &state) override;
    Control setValue(State &state, Value value) override;
};

#pragma once
#include "VariableNode.h"
#include "Token.h"

class AccessNode : public VariableNode
{
public:
    AccessNode(std::string property);
    Control resolve(State &state) override;

protected:
    std::string property;
};

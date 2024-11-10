#pragma once
#include "VariableNode.h"

class TypeNode : public VariableNode
{
public:
    TypeNode(std::string type);
    Control resolve(State &state) override;
    std::string getVariable();
    std::string getArgType();
    Node *copy() override;

private:
    std::string argType = "dyn";
};
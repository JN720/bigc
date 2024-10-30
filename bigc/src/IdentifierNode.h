#pragma once
#include "VariableNode.h"

class IdentifierNode : public VariableNode
{
public:
    // pipe result
    IdentifierNode();
    IdentifierNode(std::string var);
    IdentifierNode(Token &token);
    Control resolve(State &state) override;
};
#pragma once
#include "VariableNode.h"

class IdentifierNode : public VariableNode
{
public:
    // pipe result
    IdentifierNode();
    IdentifierNode(Token &token);
    Control resolve(State &state) override;
};
#pragma once
#include "VariableNode.h"

class IdentifierNode : public VariableNode
{
public:
    // pipe result
    IdentifierNode();
    IdentifierNode(Token &token);
    virtual std::string resolve(State &state) override;
    void makeCall();
};
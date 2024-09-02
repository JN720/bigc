#pragma once
#include "Node.h"

class IdentifierNode : public Node
{
public:
    // pipe result
    IdentifierNode();
    IdentifierNode(Token &token);
    virtual Value getValue(const State &state) override;
    void makeCall();

protected:
    std::string variable;
};
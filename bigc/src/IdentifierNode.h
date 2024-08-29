#pragma once
#include "Node.h"

class IdentifierNode : public Node
{
public:
    IdentifierNode(Token &token);
    virtual Value getValue(const State &state) override;
    void makeCall();

protected:
    std::string variable;
};
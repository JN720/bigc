#pragma once
#include "Node.h"

class IdentifierNode : public Node
{
public:
    // pipe result
    IdentifierNode();
    IdentifierNode(Token &token);
    virtual std::string resolve(State &state) override;
    std::string getVariable();
    void makeCall();

protected:
    std::string variable;
};
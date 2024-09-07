#pragma once
#include "Node.h"

class IdentifierNode : public Node
{
public:
    // pipe result
    IdentifierNode();
    IdentifierNode(Token &token);
    virtual Value getValue(const State &state) override;
    virtual std::string resolve(State &state) override;
    std::string getVariable();
    void makeCall();

protected:
    std::string variable;
};
#pragma once
#include "Node.h"

class VariableNode : public Node
{
public:
    VariableNode();
    VariableNode(std::string var);
    virtual std::string getVariable();

protected:
    std::string variable;
};
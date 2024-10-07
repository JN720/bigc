#pragma once
#include "Node.h"

class VariableNode : public Node
{
public:
    VariableNode();
    VariableNode(std::string var);
    virtual std::string getVariable();
    void setVariable(std::string name);
    virtual Control setValue(State &state, Value value);

protected:
    std::string variable;
};
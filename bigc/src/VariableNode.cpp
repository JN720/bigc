#include "VariableNode.h"

VariableNode::VariableNode()
{
}

VariableNode::VariableNode(std::string var)
{
    variable = var;
}

std::string VariableNode::getVariable()
{
    return variable;
}

void VariableNode::setVariable(std::string name)
{
    variable = name;
}

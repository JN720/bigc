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

Control VariableNode::setValue(State &state, Value value)
{
    state.setVariable(variable, value);
    return Control(OK);
}

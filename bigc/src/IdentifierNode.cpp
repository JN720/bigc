#include "IdentifierNode.h"

IdentifierNode::IdentifierNode()
{
    variable = "&";
    type = N_IDENTIFIER;
}

IdentifierNode::IdentifierNode(Token &token)
{
    variable = token.value;
    type = N_IDENTIFIER;
}

void IdentifierNode::makeCall()
{
    type = N_CALL;
}

std::string IdentifierNode::resolve(State &state)
{
    auto result = state.getVariable(variable);
    if (!result.ok())
        return "variable not found: '" + variable + '\'';
    value = result.getValue();
    return "";
}

std::string IdentifierNode::getVariable()
{
    return variable;
}

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

Value IdentifierNode::getValue(const State &state)
{
    return state.getVariable(variable);
}

std::string IdentifierNode::resolve(State &state)
{
    return std::string();
}

std::string IdentifierNode::getVariable()
{
    return variable;
}

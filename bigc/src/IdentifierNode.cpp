#include "IdentifierNode.h"

IdentifierNode::IdentifierNode(Token &token)
{
    variable = token.value;
    type = IDENTIFIER;
}

void IdentifierNode::makeCall()
{
    type = CALL;
}

Value IdentifierNode::getValue(const State &state)
{
    return state.getVariable(variable);
}
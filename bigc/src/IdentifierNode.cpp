#include "IdentifierNode.h"
#include "FunctionNode.h"
#include "ClassNode.h"

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

Control IdentifierNode::resolve(State &state)
{

    Result<Value> result = state.getVariable(variable);
    if (!result.ok())
        return Control("variable not found: '" + variable + '\'');
    value = result.getValue();
    return Control(OK);
}
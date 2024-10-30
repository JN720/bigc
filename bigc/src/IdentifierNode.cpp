#include "IdentifierNode.h"
#include "FundamentalFunctionNode.h"
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
    if (state.isBuiltIn(variable))
    {
        value = Value(new FundamentalFunctionNode(variable));
        return Control(OK);
    }
    Result<Value> result = state.getVariable(variable);
    if (!result.ok())
        return Control("variable not found: '" + variable + '\'');
    value = result.getValue();
    return Control(OK);
}

IdentifierNode::IdentifierNode(std::string var)
{
    variable = var;
    type = N_IDENTIFIER;
}
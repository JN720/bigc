#include "IdentifierNode.h"
#include "FundamentalFunctionNode.h"
#include "ClassNode.h"
#include "FunctionNode.h"

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
    Wildcard val = value.getValue();
    if (variable == "thisfn")
    {
        Node *node = (*(std::get_if<Node *>(&val)))->copy();
        Control control = node->resolve(state);
        if (control.control())
        {
            value = node->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("copying thisfn:\n");
        value = node->getValue(state);
    }
    return Control(OK);
}

IdentifierNode::IdentifierNode(std::string var)
{
    variable = var;
    type = N_IDENTIFIER;
}

Node *IdentifierNode::copy()
{
    return new IdentifierNode(variable);
}
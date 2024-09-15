#include "IdentifierNode.h"
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

void IdentifierNode::makeCall()
{
    type = N_CALL;
}

std::string IdentifierNode::resolve(State &state)
{
    auto result = state.getVariable(variable);
    if (!result.ok())
        return "variable not found: '" + variable + '\'';
    if (type == N_IDENTIFIER)
        value = result.getValue();
    // function call
    else
    {
        // ensure this is a function
        Wildcard val = result.getValue().getValue();
        FunctionNode **function = (FunctionNode **)std::get_if<Node *>(&val);
        if (!function)
            return "value is not a function";
        // prepare the arguments
        for (auto child : children)
        {
            std::string error = child->resolve(state);
            if (!error.empty())
                return "resolving function arguments:\n" + error;
        }
        result = (*function)->execute(state, children);
        if (!result.ok())
            return "calling function:\n" + result.getError();
        value = result.getValue();
    }
    return "";
}

std::string IdentifierNode::getVariable()
{
    return variable;
}

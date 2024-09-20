#include "IdentifierNode.h"
#include "FunctionNode.h"
#include "builtin.h"

const std::string FUNDAMENTAL_FUNCTIONS[] = {"print", "println", "len", "type", "input"};

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

    // check if this is built-in, user-defined, or not a function
    int builtin = -1;
    for (int f = 0; f < sizeof(FUNDAMENTAL_FUNCTIONS) / sizeof(std::string); f++)
    {
        if (variable == FUNDAMENTAL_FUNCTIONS[f])
        {
            builtin = f;
            break;
        }
    }

    if (builtin != -1)
    {
        // prepare the arguments
        for (auto child : children)
        {
            std::string error = child->resolve(state);
            if (!error.empty())
                return "resolving function arguments:\n" + error;
        }
        // execute built-in function
        Result<Value> result = base::executeFundamentalFunction(builtin, state, children);
        // check for return
        if (result.getSignal() == RETURN)
        {
            value = result.getValue();
            return "";
        }
        if (!result.ok())
            return "executing built-in function:\n" + result.getError();
        value = result.getValue();
        return "";
    }

    Result<Value> result = state.getVariable(variable);
    if (!result.ok())
        return "variable not found: '" + variable + '\'';
    if (type == N_IDENTIFIER)
        value = result.getValue();
    // function call
    else
    {
        // prepare the arguments
        for (auto child : children)
        {
            std::string error = child->resolve(state);
            if (!error.empty())
                return "resolving function arguments:\n" + error;
        }
        Wildcard val = result.getValue().getValue();
        FunctionNode **function = (FunctionNode **)std::get_if<Node *>(&val);
        if (!function)
            return "value is not a function";
        result = (*function)->execute(state, children);
        if (!result.ok())
            return "calling function:\n" + result.getError();
        // execute the function with the resolved args
        value = result.getValue();
    }
    return "";
}
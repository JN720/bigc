#include "IdentifierNode.h"
#include "FunctionNode.h"
#include "builtin.h"
#include "ClassNode.h"

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

Control IdentifierNode::resolve(State &state)
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
            Control control = child->resolve(state);
            if (control.control())
            {
                value = child->getValue(state);
                return control;
            }
            if (control.error())
                return control.stack("resolving function arguments:\n");
        }
        // execute built-in function
        Result<Value> result = base::executeFundamentalFunction(builtin, state, children);
        // check for return
        if (result.ok())
        {
            value = result.getValue();
            return Control(OK);
        }
        return Control(result.getError()).stack("executing built-in function:\n");
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
            Control control = child->resolve(state);
            if (control.control())
            {
                value = child->getValue(state);
                return control;
            }
            if (control.error())
                return control.stack("resolving function arguments:\n");
        }
        Wildcard val = result.getValue().getValue();
        Node **node = std::get_if<Node *>(&val);
        if (!node)
            return Control("value is not a function or class");
        if (FunctionNode *function = dynamic_cast<FunctionNode *>(*node))
        {
            // execute the function with the resolved args
            result = function->execute(state, children);
            if (!result.ok())
                return "calling function:\n" + result.getError();
        }
        else if (ClassNode *objClass = dynamic_cast<ClassNode *>(*node))
        {
            result = objClass->getClassDefinition()->construct(&state, children);
            if (!result.ok())
                return "constructing object:\n" + result.getError();
        }
        else
            return Control("value is not a function or a class");
        value = result.getValue();
    }
    return Control(OK);
}
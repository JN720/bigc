#include "CallNode.h"
#include "IdentifierNode.h"
#include "FundamentalFunctionNode.h"
#include "ClassNode.h"
#include "Object.h"
#include "builtin.h"
#include "MethodNode.h"

CallNode::CallNode(Node *callable) : Node()
{
    children.push_back(callable);
    type = N_CALL;
}

Control CallNode::resolve(State &state)
{
    if (children.size() < 1)
        return Control("insufficient children for call");

    // resolve the callable
    Control control = children.front()->resolve(state);
    if (control.control())
    {
        value = children.front()->getValue(state);
        return control;
    }
    if (control.error())
        return control.stack("Attempted to call a non-function value");

    Wildcard callable = children.front()->getValue(state).getValue();

    Node **node = std::get_if<Node *>(&callable);
    if (!node)
        return Control("value is not a function or class");
    std::vector<Node *> args = std::vector<Node *>(children.begin() + 1, children.end());

    // check if it is a function
    if (FunctionNode *function = dynamic_cast<FunctionNode *>(*node))
    {
        // execute the function with the resolved args
        Result<Value> result = function->execute(state, args);
        if (!result.ok())
            return Control(result.getError()).stack("calling function:\n");
        value = result.getValue();
        return Control(OK);
    }
    // if it is a class, construct an object
    else if (ClassNode *objClass = dynamic_cast<ClassNode *>(*node))
    {
        Result<Value> result = objClass->getClassDefinition()->construct(&state, args);
        if (!result.ok())
            return Control(result.getError()).stack("constructing object:\n");
        value = result.getValue();
        return Control(OK);
    }
    // if this is an object check for a call method
    /*
    else if (Object *obj = dynamic_cast<Object *>(*node))
    {
        Result<Value> result = obj->callMethod(state, args);
        if (!result.ok())
            return Control(result.getError()).stack("calling method:\n");
        value = result.getValue();
        return Control(OK);
    }*/
    else
        return Control("value is not a function, class, or callable object");
}
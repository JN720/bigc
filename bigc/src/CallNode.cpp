#include "CallNode.h"
#include "IdentifierNode.h"
#include "MethodNode.h"
#include "ClassNode.h"
#include "Object.h"
#include "builtin.h"
#include "MethodNode.h"
#include "InterfaceNode.h"

CallNode::CallNode()
{
    type = N_CALL;
}

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
        return control.stack("failed to resolve callable");

    Wildcard callable = children.front()->getValue(state).getValue();
    std::vector<Node *> args = std::vector<Node *>(children.begin() + 1, children.end());
    // resolve the arguments
    for (Node *arg : args)
    {
        Control control = arg->resolve(state);
        if (control.control())
        {
            value = arg->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("resolving arguments:\n");
    }

    Node **node = std::get_if<Node *>(&callable);
    if (!node)
    {
        Object **callableObj = std::get_if<Object *>(&callable);
        Object *obj = *callableObj;
        if (!obj)
            return Control("value is not a function, class, or callable object");
        // check that the interface exists
        Result<Value> interfaceResult = state.getVariable("Callable");
        if (!interfaceResult.ok())
            return Control("Callable interface is not defined");
        Wildcard val = interfaceResult.getValue().getValue();
        // ensure it is a node
        Node **callableNode = std::get_if<Node *>(&val);
        if (!callableNode)
            return Control("Callable interface is not a node");
        // ensure it is an interface
        InterfaceNode *interfaceNode = dynamic_cast<InterfaceNode *>(*callableNode);
        if (!interfaceNode)
            return Control("Callable interface is not an interface");
        Interface *interface = interfaceNode->getInterface();
        // check that the object implements the interface
        if (!static_cast<ClassDefinition *>(obj->getClass())->implements(interface))
            return Control("object does not implement Callable interface");
        // check that the object has a call method
        Result<Node *> methodResult = static_cast<ClassDefinition *>(obj->getClass())->getClassMethod("call");
        if (!methodResult.ok())
            return Control(methodResult.getError()).stack("calling method:\n");
        // ensure it is a function
        FunctionNode *function = dynamic_cast<FunctionNode *>(methodResult.getValue());
        if (!function)
            return Control("Callable method is not a function");
        // execute the method
        if (!dynamic_cast<MethodNode *>(function))
            function = new MethodNode(function, obj, state);
        Result<Value> result = function->execute(state, args);
        if (!result.ok())
            return Control(result.getError()).stack("calling method:\n");
        value = result.getValue();
        return Control(OK);
    }

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
    else
        return Control("value is not a function, class, or callable object");
}

Node *CallNode::copy()
{
    CallNode *call = new CallNode();
    for (auto child : children)
    {
        call->addChild(child->copy());
    }
    return call;
}

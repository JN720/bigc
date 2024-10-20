#include "MethodNode.h"

MethodNode::MethodNode()
{
}

MethodNode::MethodNode(Node *method, Object *object)
{
    this->object = object;
    children = method->getChildren();
}

Result<Value> MethodNode::execute(State &state, std::vector<Node *> &args)
{
    return executeInstanced(object, &state, args);
}

Control MethodNode::resolve(State &state)
{
    if (children.size() < 1)
        return Control("function must have sequence");
    value = Value(this);
    return Control(OK);
}

#include "MethodNode.h"

MethodNode::MethodNode(Node *method, Object *object)
{
    this->object = object;
    this->method = method;
}

Result<Value> MethodNode::execute(State &state, std::vector<Node *> &args)
{
    return static_cast<FunctionNode *>(method)->executeInstanced(object, &state, args);
}

Control MethodNode::resolve(State &state)
{
    if (children.size() < 1)
        return Control("function must have sequence");
    value = Value(this);
    return Control(OK);
}

void MethodNode::setObject(Object *obj)
{
    object = obj;
}

MethodNode::MethodNode()
{
}

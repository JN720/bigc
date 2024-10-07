#include "MethodNode.h"

MethodNode::MethodNode(Node *method, Object *object)
{
    this->object = object;
    children.push_back(method);
}

Result<Value> MethodNode::execute(State &state, std::vector<Node *> &args)
{
    return executeInstanced(object, &state, args);
}

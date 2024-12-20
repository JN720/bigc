#include "MethodNode.h"

MethodNode::MethodNode(Node *method, Object *object, State &state)
{
    this->object = object;
    this->method = method;
    state.addRef(object);
    state.addRef(method);
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

void MethodNode::destroy(State *state)
{
    state->removeRef(object);
    state->removeRef(method);
    Node::destroy(state);
}

MethodNode::MethodNode()
{
}

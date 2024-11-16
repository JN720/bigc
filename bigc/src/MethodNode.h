#pragma once

#include "FunctionNode.h"

// this encapsulates a method call and includes the object it is called on
// it is used for instanced methods
// this takes the place of a function node for methods
class MethodNode : public FunctionNode
{
public:
    MethodNode(Node *method, Object *object);
    Result<Value> execute(State &state, std::vector<Node *> &args) override;
    Control resolve(State &state) override;
    void setObject(Object *obj);
    virtual void destroy(State *state) override;

protected:
    MethodNode();
    Object *object;
    Node *method;
};

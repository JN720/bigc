#pragma once

#include "FunctionNode.h"
#include "ClassDefinition.h"

// this encapsulates a method call and includes the object it is called on
// it is used for instanced methods
// this takes the place of a function node for methods
class StaticMethodNode : public FunctionNode
{
public:
    StaticMethodNode(Node *method, ClassNode *cls, State &state);
    Result<Value> execute(State &state, std::vector<Node *> &args) override;
    Control resolve(State &state) override;
    virtual void destroy(State *state) override;

private:
    ClassNode *cls;
};

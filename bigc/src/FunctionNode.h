#pragma once
#include "Node.h"

// the arguments are first then the sequence
class FunctionNode : public Node
{
public:
    FunctionNode();
    Control resolve(State &state) override;
    virtual Result<Value> execute(State &state, std::vector<Node *> &args);
    Control resolveArguments(State &state, std::vector<Node *> &args);
    virtual std::string getFunctionSignature();
    Node *copy() override;

    // only use in method node
    virtual Result<Value> executeInstanced(Object *obj, State *state, std::vector<Node *> &args);

protected:
    Result<std::vector<Value>> valuifyArgs(std ::vector<Node *> &args, State &state);
};

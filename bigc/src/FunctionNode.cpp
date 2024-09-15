#include "FunctionNode.h"
#include "IdentifierNode.h"
#include "TypeNode.h"

FunctionNode::FunctionNode()
{
    type = N_LEAFVALUE;
}

std::string FunctionNode::resolve(State &state)
{
    if (children.size() < 1)
        return "function must have sequence";
    value = Value(this);
    return "";
}

Result<Value> FunctionNode::execute(State &state, std::vector<Node *> &args)
{
    // each arg should be an identifier or type node
    // the final one can also be a spread node
    StateFrame *frame = state.pushFrame();
    for (int i = 0; i < args.size() - 1; i++)
    {
        auto arg = args[i];
        Value val = arg->getValue(state);
        if (dynamic_cast<IdentifierNode *>(arg))
            frame->setVariable(((IdentifierNode *)arg)->getVariable(), val);
        else if (dynamic_cast<TypeNode *>(arg))
        {
            TypeNode *typed = (TypeNode *)arg;
            if (typed->getArgType() != val.getType())
                return Result<Value>("type assertion failed");
            frame->setVariable(typed->getName(), val);
        }
        else
            return Result<Value>("invalid node in function arguments");
    }
    std::string error = children.back()->resolve(state);
    if (!error.empty())
        return Result<Value>("during function execution:\n" + error);
    Value result = children.back()->getValue(state);
    state.popFrame();
    return Result<Value>(result);
}
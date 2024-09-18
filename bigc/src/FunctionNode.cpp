#include "FunctionNode.h"
#include "TypeNode.h"
#include "SpreadNode.h"

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
    // each arg should be a variable node or type node
    // the final one can also be a spread node
    // create the frame for the arguments we pass in
    StateFrame *frame = state.pushFrame();
    // take each arg, do type assertions, and add to the frame
    int curVal = 0;
    int curArg = 0;
    // place arguments until we hit the sequence
    while (curArg < children.size() - 1 && curVal < args.size())
    {
        Node *arg = args[curVal];
        Node *child = children[curArg];
        // there should be one more child than args because the last is the end
        // we have already resolved it so get the value
        Value val = args[curVal]->getValue(state);
        if (dynamic_cast<VariableNode *>(child))
        {
            // get the name of the function arg and set its value to val
            frame->setVariable(((VariableNode *)child)->getVariable(), val);
            curVal++;
            curArg++;
        }
        else if (dynamic_cast<TypeNode *>(child))
        {
            TypeNode *typed = (TypeNode *)arg;
            if (typed->getArgType() != val.getType())
                return Result<Value>("type assertion failed");
            frame->setVariable(typed->getVariable(), val);
            curVal++;
            curArg++;
        }
        /*
        else if (dynamic_cast<SpreadNode *>(child))
        {
            SpreadNode *spread = (SpreadNode *)arg;
            if (dynamic_cast<IdentifierNode *>(child)) {
                spread->get
            }
            // go to the next value but stay on the same arg
            curVal++;
        }*/
        else
            return Result<Value>("invalid node in function arguments");
    }
    // resolve the sequence node
    std::string error = children.back()->resolve(state);
    if (!error.empty())
        return Result<Value>("during function execution:\n" + error);
    Value result = children.back()->getValue(state);
    state.popFrame();
    return Result<Value>(result);
}
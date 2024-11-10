#include "FundamentalFunctionNode.h"
#include "builtin.h"

const std::unordered_map<std::string, int> FUNCTION_MAP = {
    {"print", 0},
    {"println", 1},
    {"len", 2},
    {"type", 3},
    {"input", 4},
    {"import", 5},
    {"include", 6}};

FundamentalFunctionNode::FundamentalFunctionNode(const std::string &name)
{
    functionIndex = FUNCTION_MAP.find(name)->second;
}

Result<Value> FundamentalFunctionNode::execute(State &state, std::vector<Node *> &args)
{
    // resolve the arguments
    for (Node *arg : args)
    {
        Control control = arg->resolve(state);
        if (control.control())
        {
            value = arg->getValue(state);
            return control;
        }
        if (!control.ok())
            return control.stack("resolving argument:\n");
    }
    return base::executeFundamentalFunction(functionIndex, state, args);
}

Node *FundamentalFunctionNode::copy()
{
    return new FundamentalFunctionNode(std::string(this->getFunctionSignature()));
}

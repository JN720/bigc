#include "FundamentalFunctionNode.h"
#include "builtin.h"

const std::string FUNDAMENTAL_FUNCTIONS[] = {"print", "println", "len", "type", "input"};
const std::unordered_map<std::string, int> FUNCTION_MAP = {
    {"print", 0},
    {"println", 1},
    {"len", 2},
    {"type", 3},
    {"input", 4}};

FundamentalFunctionNode::FundamentalFunctionNode(const std::string &name)
{
    functionIndex = FUNCTION_MAP.find(name)->second;
}

Result<Value> FundamentalFunctionNode::execute(State &state, std::vector<Node *> &args)
{
    return base::executeFundamentalFunction(functionIndex, state, args);
}
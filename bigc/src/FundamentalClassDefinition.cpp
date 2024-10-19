#include "FundamentalClassDefinition.h"

Result<Value> FundamentalClassDefinition::construct(State *state, std::vector<Node *> &args)
{
    return Result<Value>("FundamentalClassDefinition cannot be instantiated");
}

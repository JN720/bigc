#pragma once

#include "ClassDefinition.h"
#include "FundamentalMethodNode.h"

// this is the superclass for all fundamental classes
// each fundamental class must implement the construct method
// subclasses will have FundamentalMethodNode methods
class FundamentalClassDefinition : public ClassDefinition
{
public:
    Result<Value> construct(State *state, std::vector<Node *> &args) override;
};

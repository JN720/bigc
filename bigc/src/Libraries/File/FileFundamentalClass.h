#pragma once
#include "../../FundamentalClassDefinition.h"
#include "FileFundamentalObject.h"

class FileFundamentalClass : public FundamentalClassDefinition
{
public:
    FileFundamentalClass();

    Result<Value> construct(State *state, std::vector<Node *> &args) override;
};
#pragma once
#include "../../FundamentalClassDefinition.h"
#include "FileFundamentalObject.h"

#if defined(BUILD_CPU) || defined(BUILD_GPU)
class FileFundamentalClass : public FundamentalClassDefinition
{
public:
    FileFundamentalClass();

    Result<Value> construct(State *state, std::vector<Node *> &args) override;
};
#endif
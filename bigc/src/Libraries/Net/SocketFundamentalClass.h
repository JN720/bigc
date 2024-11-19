#pragma once
#include "../../FundamentalClassDefinition.h"
#include "SocketFundamentalObject.h"

#if defined(BUILD_CPU) || defined(BUILD_GPU)
class SocketFundamentalClass : public FundamentalClassDefinition
{
public:
    SocketFundamentalClass();
    Result<Value> construct(State *state, std::vector<Node *> &args) override;
};
#endif // BUILD_CPU || BUILD_GPU

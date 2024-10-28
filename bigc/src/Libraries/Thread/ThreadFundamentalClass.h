#pragma once
#include "../../FundamentalClassDefinition.h"
#include "ThreadFundamentalObject.h"

namespace libthread
{
    class ThreadFundamentalClass : public FundamentalClassDefinition
    {
    public:
        ThreadFundamentalClass();

        Result<Value> construct(State *state, std::vector<Node *> &args) override;
    };
}
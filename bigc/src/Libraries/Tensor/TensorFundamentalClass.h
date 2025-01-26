#pragma once
#include "../../FundamentalClassDefinition.h"
#include "TensorFundamentalObject.h"

namespace libtensor
{
    class TensorFundamentalClass : public FundamentalClassDefinition
    {
    public:
        TensorFundamentalClass();

        Result<Value> construct(State *state, std::vector<Node *> &args) override;
    };
}
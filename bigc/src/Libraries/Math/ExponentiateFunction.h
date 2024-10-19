#pragma once

#include "../../FunctionNode.h"

namespace libmath
{
    class ExponentiateFunction : public FunctionNode
    {
    public:
        ExponentiateFunction();
        Result<Value> execute(State &state, std::vector<Node *> &args) override;
    };
}
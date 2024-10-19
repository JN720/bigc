#pragma once
#include "../../FunctionNode.h"

namespace libmath
{
    class SqrtFunction : public FunctionNode
    {
    public:
        SqrtFunction();
        Result<Value> execute(State &state, std::vector<Node *> &args) override;
    };
}

#pragma once

#include "../../FunctionNode.h"
#include <functional>

namespace libmath
{
    class GeneralMathFunction : public FunctionNode
    {
    public:
        using MathLambda = std::function<Result<Value>(State &, std::vector<Node *> &)>;

        GeneralMathFunction(MathLambda lambda);
        Result<Value> execute(State &state, std::vector<Node *> &args) override;

    private:
        MathLambda lambda;
    };
}

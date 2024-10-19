#include "GeneralMathFunction.h"

namespace libmath
{
    GeneralMathFunction::GeneralMathFunction(MathLambda lambda)
        : lambda(lambda)
    {
    }

    Result<Value> GeneralMathFunction::execute(State &state, std::vector<Node *> &args)
    {
        return lambda(state, args);
    }
}

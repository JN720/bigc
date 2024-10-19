#include "SqrtFunction.h"
#include <cmath>

libmath::SqrtFunction::SqrtFunction() {}

Result<Value> libmath::SqrtFunction::execute(State &state, std::vector<Node *> &args)
{
    if (args.size() != 1)
        return Result<Value>("invalid arity");
    Wildcard arg = args[0]->getValue(state).getValue();
    if (double *x = std::get_if<double>(&arg))
        return Result<Value>(Value(std::sqrt(*x)));
    else if (int *x = std::get_if<int>(&arg))
        return Result<Value>(Value(std::sqrt(static_cast<double>(*x))));
    else if (float *x = std::get_if<float>(&arg))
        return Result<Value>(Value(std::sqrt(*x)));
    else
        return Result<Value>("expected number argument");
}

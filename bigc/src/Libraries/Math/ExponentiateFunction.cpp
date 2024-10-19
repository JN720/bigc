#include "ExponentiateFunction.h"

#include <cmath>

libmath::ExponentiateFunction::ExponentiateFunction()
{
}

Result<Value> libmath::ExponentiateFunction::execute(State &state, std::vector<Node *> &args)
{
    if (args.size() != 2)
        return Result<Value>("invalid arity");
    Wildcard base = args[0]->getValue(state).getValue();
    Wildcard exponent = args[1]->getValue(state).getValue();
    if (double *x = std::get_if<double>(&base))
    {
        if (double *y = std::get_if<double>(&exponent))
            return Result<Value>(Value(std::pow(*x, *y)));
        else
            return Result<Value>("expected double exponent");
    }
    else if (int *x = std::get_if<int>(&base))
    {
        if (int *y = std::get_if<int>(&exponent))
            return Result<Value>(Value(std::pow(*x, *y)));
        else
            return Result<Value>("expected int exponent");
    }
    else if (float *x = std::get_if<float>(&base))
    {
        if (float *y = std::get_if<float>(&exponent))
            return Result<Value>(Value(std::pow(*x, *y)));
        else
            return Result<Value>("expected float exponent");
    }
    else
        return Result<Value>("expected number base");
}
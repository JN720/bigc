#include "TrigFunctions.h"
#include <cmath>

namespace libmath
{

    Result<Value> executeTrigFunction(State &state, std::vector<Node *> &args, double (*func)(double))
    {
        if (args.size() != 1)
            return Result<Value>("invalid arity");
        Wildcard arg = args[0]->getValue(state).getValue();
        if (double *x = std::get_if<double>(&arg))
            return Result<Value>(Value(func(*x)));
        else if (int *x = std::get_if<int>(&arg))
            return Result<Value>(Value(func(static_cast<double>(*x))));
        else if (float *x = std::get_if<float>(&arg))
            return Result<Value>(Value(func(*x)));
        else
            return Result<Value>("expected number argument");
    }

    SinFunction::SinFunction() {}
    Result<Value> SinFunction::execute(State &state, std::vector<Node *> &args)
    {
        return executeTrigFunction(state, args, std::sin);
    }

    CosFunction::CosFunction() {}
    Result<Value> CosFunction::execute(State &state, std::vector<Node *> &args)
    {
        return executeTrigFunction(state, args, std::cos);
    }

    TanFunction::TanFunction() {}
    Result<Value> TanFunction::execute(State &state, std::vector<Node *> &args)
    {
        return executeTrigFunction(state, args, std::tan);
    }

}

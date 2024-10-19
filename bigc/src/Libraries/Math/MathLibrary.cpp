#include "MathLibrary.h"
#include "ExponentiateFunction.h"
#include "SqrtFunction.h"
#include "TrigFunctions.h"
#include <cmath>

Registry *libmath::init()
{
    Registry *registry = new Registry();

    // Constants
    registry->registerVariable("pi", Value(M_PI));
    registry->registerVariable("e", Value(M_E));

    // Functions
    registry->registerVariable("exp", Value(new ExponentiateFunction()));
    registry->registerVariable("sqrt", Value(new SqrtFunction()));
    registry->registerVariable("sin", Value(new SinFunction()));
    registry->registerVariable("cos", Value(new CosFunction()));
    registry->registerVariable("tan", Value(new TanFunction()));

    // Additional mathematical functions
    registry->registerVariable("abs", Value([](State &state, std::vector<Node *> &args) -> Result<Value>
                                            {
        if (args.size() != 1) return Result<Value>("invalid arity");
        Wildcard arg = args[0]->getValue(state).getValue();
        if (double *x = std::get_if<double>(&arg)) return Result<Value>(Value(std::abs(*x)));
        if (int *x = std::get_if<int>(&arg)) return Result<Value>(Value(std::abs(*x)));
        if (float *x = std::get_if<float>(&arg)) return Result<Value>(Value(std::abs(*x)));
        return Result<Value>("expected number argument"); }));

    registry->registerVariable("log", Value([](State &state, std::vector<Node *> &args) -> Result<Value>
                                            {
        if (args.size() != 1) return Result<Value>("invalid arity");
        Wildcard arg = args[0]->getValue(state).getValue();
        if (double *x = std::get_if<double>(&arg)) return Result<Value>(Value(std::log(*x)));
        if (int *x = std::get_if<int>(&arg)) return Result<Value>(Value(std::log(static_cast<double>(*x))));
        if (float *x = std::get_if<float>(&arg)) return Result<Value>(Value(std::log(*x)));
        return Result<Value>("expected number argument"); }));

    registry->registerVariable("log10", Value([](State &state, std::vector<Node *> &args) -> Result<Value>
                                              {
        if (args.size() != 1) return Result<Value>("invalid arity");
        Wildcard arg = args[0]->getValue(state).getValue();
        if (double *x = std::get_if<double>(&arg)) return Result<Value>(Value(std::log10(*x)));
        if (int *x = std::get_if<int>(&arg)) return Result<Value>(Value(std::log10(static_cast<double>(*x))));
        if (float *x = std::get_if<float>(&arg)) return Result<Value>(Value(std::log10(*x)));
        return Result<Value>("expected number argument"); }));

    registry->registerVariable("ceil", Value([](State &state, std::vector<Node *> &args) -> Result<Value>
                                             {
        if (args.size() != 1) return Result<Value>("invalid arity");
        Wildcard arg = args[0]->getValue(state).getValue();
        if (double *x = std::get_if<double>(&arg)) return Result<Value>(Value(std::ceil(*x)));
        if (int *x = std::get_if<int>(&arg)) return Result<Value>(Value(*x));
        if (float *x = std::get_if<float>(&arg)) return Result<Value>(Value(std::ceil(*x)));
        return Result<Value>("expected number argument"); }));

    registry->registerVariable("floor", Value([](State &state, std::vector<Node *> &args) -> Result<Value>
                                              {
        if (args.size() != 1) return Result<Value>("invalid arity");
        Wildcard arg = args[0]->getValue(state).getValue();
        if (double *x = std::get_if<double>(&arg)) return Result<Value>(Value(std::floor(*x)));
        if (int *x = std::get_if<int>(&arg)) return Result<Value>(Value(*x));
        if (float *x = std::get_if<float>(&arg)) return Result<Value>(Value(std::floor(*x)));
        return Result<Value>("expected number argument"); }));

    return registry;
}

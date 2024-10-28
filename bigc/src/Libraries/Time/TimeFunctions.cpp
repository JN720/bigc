#include "TimeFunctions.h"
#include <thread>
#include <chrono>

Result<Value> libtime::sleep(State &state, std::vector<Node *> &args)
{
    if (args.empty())
        return Result<Value>("no duration provided");

    Wildcard val = args[0]->getValue(state).getValue();
    int durationMs = 0;
    if (int *x = std::get_if<int>(&val))
        durationMs = *x;
    else if (long *x = std::get_if<long>(&val))
        durationMs = *x;
    else if (float *x = std::get_if<float>(&val))
        durationMs = (int)(*x);
    else if (double *x = std::get_if<double>(&val))
        durationMs = (int)(*x);
    else
        return Result<Value>("argument must be a number");
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
    return Result<Value>(args[0]->getValue(state));
}
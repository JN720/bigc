#include "ThreadFunctions.h"
#include "ThreadFundamentalClass.h"
#include <future>

Result<Value> libthread::awaitFunction(State &state, std::vector<Node *> &args)
{
    if (args.size() != 1)
        return Result<Value>("await requires one argument: thread");

    Control control = args[0]->resolve(state);
    if (control.control())
        return Result<Value>(control);
    if (control.error())
        return Result<Value>("resolving arguments:\n" + control.error());
    Wildcard val = args[0]->getValue(state).getValue();
    if (Object **obj = std::get_if<Object *>(&val))
    {
        if (ThreadFundamentalObject *threadObj = dynamic_cast<ThreadFundamentalObject *>(*obj))
        {
            Result<Value> result = threadObj->getResult();
            return result;
        }
        else
            return Result<Value>("expected first argument to be thread");
    }
    else
        return Result<Value>("expected first argument to be thread object");
}

Result<Value> libthread::asyncFunction(State &state, std::vector<Node *> &args)
{
    if (args.size() != 1)
        return Result<Value>("async requires one argument: method");
    return Result<Value>(Value(new ThreadFundamentalObject(args[0], &state)));
}

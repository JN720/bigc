#include "ThreadFundamentalClass.h"

libthread::ThreadFundamentalClass::ThreadFundamentalClass()
{
}

Result<Value> libthread::ThreadFundamentalClass::construct(State *state, std::vector<Node *> &args)
{
    if (args.size() != 1)
        return Result<Value>("Thread constructor requires one argument");

    Object *object = new ThreadFundamentalObject(args[0], state);
    return Result<Value>(Value(object));
}

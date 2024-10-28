#include "ThreadFundamentalObject.h"

libthread::ThreadFundamentalObject::ThreadFundamentalObject(Node *method, State *state)
{
    curThread = std::thread([this, method, state]()
                            {
                                State stateRef = *state; 
                                Control control = method->resolve(stateRef);
                                if (!control.ok())
                                {
                                    result.set_value(Result<Value>(control.stack("executing thread:\n")));
                                    return;                                                               
                                }
                                result.set_value(method->getValue(stateRef)); });
}

libthread::ThreadFundamentalObject::~ThreadFundamentalObject()
{
}

Result<Value> libthread::ThreadFundamentalObject::getProperty(const std::string &property)
{
    return Result<Value>("cannot access property of thread");
}

Control libthread::ThreadFundamentalObject::setProperty(const std::string &property, const Value &value)
{
    return Control("cannot set property of thread");
}

std::thread libthread::ThreadFundamentalObject::getThread()
{
    return std::move(curThread);
}

Result<Value> libthread::ThreadFundamentalObject::getResult()
{
    return result.get_future().get();
}

#include "ThreadFundamentalObject.h"

libthread::ThreadFundamentalObject::ThreadFundamentalObject(Node *method, State *state)
{
    result = Result<Value>(Value(79));
    curThread = std::thread([this, method, state]()
                            {
                                State stateRef = *state; 
                                Control control = method->resolve(stateRef);
                                if (!control.ok())
                                {
                                    promise.set_value(Result<Value>(control.stack("executing thread:\n")));
                                    return;                                                               
                                }
                                this->promise.set_value(method->getValue(stateRef)); });
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

std::thread &libthread::ThreadFundamentalObject::getThread()
{
    return curThread;
}

Result<Value> libthread::ThreadFundamentalObject::getResult()
{

    curThread.join();
    result = promise.get_future().get();
    return result;
}
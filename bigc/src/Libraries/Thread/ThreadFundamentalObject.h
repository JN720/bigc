#pragma once
#include "../../FundamentalObject.h"
#include <thread>
#include <future>

namespace libthread
{
    class ThreadFundamentalObject : public FundamentalObject
    {
    public:
        ThreadFundamentalObject(Node *method, State *state);
        ~ThreadFundamentalObject();

        Result<Value> getProperty(const std::string &property) override;
        Control setProperty(const std::string &property, const Value &value) override;
        std::thread getThread();
        Result<Value> getResult();

    private:
        std::thread curThread;
        std::promise<Result<Value>> result;
    };
}
#pragma once
#include "../../FundamentalObject.h"
#include "Tensor.h"

namespace libtensor
{
    class TensorFundamentalObject : public FundamentalObject
    {
    public:
        TensorFundamentalObject(Node *method, State *state);
        ~TensorFundamentalObject();

        Result<Value> getProperty(const std::string &property) override;
        Control setProperty(const std::string &property, const Value &value) override;
        Result<Value> getResult();

    private:
        std::vector<int> dims;
        Tensor tensor;
    };
}
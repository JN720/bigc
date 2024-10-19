#pragma once
#include "FunctionNode.h"

namespace libmath
{
    class SinFunction : public FunctionNode
    {
    public:
        SinFunction();
        Result<Value> execute(State &state, std::vector<Node *> &args) override;
    };

    class CosFunction : public FunctionNode
    {
    public:
        CosFunction();
        Result<Value> execute(State &state, std::vector<Node *> &args) override;
    };

    class TanFunction : public FunctionNode
    {
    public:
        TanFunction();
        Result<Value> execute(State &state, std::vector<Node *> &args) override;
    };
}

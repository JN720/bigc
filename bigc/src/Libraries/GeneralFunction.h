#pragma once

#include "../FunctionNode.h"
#include <functional>
namespace lib
{
    class GeneralFunction : public FunctionNode
    {
    public:
        using Lambda = std::function<Result<Value>(State &, std::vector<Node *> &)>;

        GeneralFunction(Lambda lambda);
        Result<Value> execute(State &state, std::vector<Node *> &args) override;

    private:
        Lambda lambda;
    };
}
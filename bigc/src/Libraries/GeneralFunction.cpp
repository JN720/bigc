#include "GeneralFunction.h"

namespace lib
{
    GeneralFunction::GeneralFunction(Lambda lambda)
        : lambda(lambda)
    {
    }

    Result<Value> GeneralFunction::execute(State &state, std::vector<Node *> &args)
    {
        return lambda(state, args);
    }
}
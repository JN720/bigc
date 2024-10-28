#pragma once

#include "../../Registry.h"
#include "../../State.h"
#include "../../Node.h"

namespace libthread
{
    Result<Value> awaitFunction(State &state, std::vector<Node *> &args);
    Result<Value> asyncFunction(State &state, std::vector<Node *> &args);
}
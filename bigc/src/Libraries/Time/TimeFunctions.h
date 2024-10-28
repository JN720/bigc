#pragma once

#include "../../Registry.h"
#include "../../State.h"
#include "../../Node.h"

namespace libtime
{

    Result<Value> sleep(State &state, std::vector<Node *> &args);
}
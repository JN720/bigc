#pragma once

#include "Node.h"
#include "Value.h"
#include "Control.h"
#include "State.h"
#include <vector>

// the first node will be the callable, and the rest will be the arguments
class CallNode : public Node
{
public:
    CallNode(Node *callable);
    Control resolve(State &state) override;
    Node *copy() override;
    // this is only for copying
private:
    CallNode();
};

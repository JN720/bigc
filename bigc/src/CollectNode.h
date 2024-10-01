#pragma once
#include "Node.h"

// first child is true, second is false
// this is very similar to a regular loop, but collects everything into a list
class CollectNode : public Node
{
public:
    CollectNode();
    Control resolve(State &state) override;
};
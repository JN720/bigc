#pragma once
#include "Node.h"

class PipeNode : public Node
{
public:
    PipeNode();
    Control resolve(State &state) override;
};

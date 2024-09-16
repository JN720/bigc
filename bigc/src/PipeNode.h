#pragma once
#include "Node.h"

class PipeNode : public Node
{
public:
    PipeNode();
    std::string resolve(State &state) override;
};

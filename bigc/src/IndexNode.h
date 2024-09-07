#pragma once
#include "Node.h"

class IndexNode : public Node
{
public:
    IndexNode();
    Value getValue(const State &state) override;
    std::string resolve(State &state) override;
};
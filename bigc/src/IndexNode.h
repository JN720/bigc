#pragma once
#include "Node.h"

class IndexNode : public Node
{
public:
    IndexNode();
    std::string resolve(State &state) override;
};
#pragma once
#include "Node.h"

class IndexNode : public Node
{
public:
    IndexNode();
    virtual Value getValue(const State &state) override;
};
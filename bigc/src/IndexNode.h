#pragma once
#include "Node.h"

class IndexNode : public Node
{
public:
    IndexNode();
    Control resolve(State &state) override;
};
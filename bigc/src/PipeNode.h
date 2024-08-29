#pragma once
#include "Node.h"

class PipeNode: public Node {
public:
    PipeNode();
    Value getValue(const State &state) override;

};

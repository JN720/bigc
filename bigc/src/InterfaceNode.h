#pragma once
#include "Node.h"

class InterfaceNode : public Node
{
public:
    InterfaceNode();
    Control resolve(State &state) override;
    Interface *getInterface();
private:
};
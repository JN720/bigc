#pragma once
#include "Node.h"
#include "Interface.h"

class InterfaceNode : public Node
{
public:
    InterfaceNode();
    Control resolve(State &state) override;
    Interface *getInterface();

private:
    Interface *interface;
};
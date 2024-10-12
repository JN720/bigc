#pragma once
#include "Node.h"
#include "Interface.h"

/*
    interface declarations look like this:
    interface Name (
        methodName@Type,
        ...
    )
*/

class InterfaceNode : public Node
{
public:
    InterfaceNode();
    Control resolve(State &state) override;
    Interface *getInterface();

private:
    Interface *interface;
};
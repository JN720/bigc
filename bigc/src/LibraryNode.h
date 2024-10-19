#pragma once

#include "Node.h"
#include "Registry.h"

class LibraryNode : public Node
{
public:
    LibraryNode(Registry *registry);
    Control resolve(State &state);
    Registry *getRegistry();

private:
    Registry *registry;
};

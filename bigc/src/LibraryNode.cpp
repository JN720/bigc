#include "LibraryNode.h"

LibraryNode::LibraryNode(Registry *registry)
{
    type = N_LIBRARY;
    this->registry = registry;
}

Registry *LibraryNode::getRegistry()
{
    return registry;
}

Control LibraryNode::resolve(State &state)
{

    value = Value(this);
    return Control(OK);
}
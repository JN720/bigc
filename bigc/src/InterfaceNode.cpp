#include "InterfaceNode.h"

InterfaceNode::InterfaceNode()
{
    type = N_INTERFACE;
}

Control InterfaceNode::resolve(State &state)
{
    // the children for this should be identifiers of methods
    return Control("not implemented");
}

Interface *InterfaceNode::getInterface()
{
    return interface;
}

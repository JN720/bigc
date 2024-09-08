#pragma once
#include "Node.h"

/*
    Functionally, this is just a sequence node that is
    marked to be the final node in a branch node's
    children and if found, will cause a branch node
    to execute it if the condition is false
*/
class UnbranchNode : public Node
{
public:
    UnbranchNode();
    std::string resolve(State &state);
};

#include "AccessNode.h"

AccessNode::AccessNode(std::string property)
{
    this->property = property;
    type = N_ACCESS;
}

Control AccessNode::resolve(State &state)
{
    return Control();
}

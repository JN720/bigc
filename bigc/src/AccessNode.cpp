#include "AccessNode.h"

AccessNode::AccessNode(std::string property)
{
    this->property = property;
    type = N_ACCESS;
}
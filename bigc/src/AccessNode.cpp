#include "AccessNode.h"

AccessNode::AccessNode(std::string property)
{
    this->property = property;
}

Value AccessNode::getValue(const State &state)
{
    return Value();
}

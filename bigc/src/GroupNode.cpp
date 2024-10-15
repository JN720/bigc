#include "GroupNode.h"

GroupNode::GroupNode(std::string name)
{
    type = N_GROUP;
    this->name = name;
}

Control GroupNode::resolve(State &state)
{
    if (hasChildren())
        return Control("group cannot have children");
    state.setGroup(name);
    return Control(OK);
}
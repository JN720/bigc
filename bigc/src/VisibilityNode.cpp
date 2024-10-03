#include "VisibilityNode.h"
#include "IdentifierNode.h"

VisibilityNode::VisibilityNode()
{
    type = N_VISIBILITY;
    access = PUBLIC;
    isStatic = false;
}

VisibilityNode::VisibilityNode(AccessSpecifier specifier)
{
    type = N_VISIBILITY;
    access = specifier;
    isStatic = false;
}

VisibilityNode::VisibilityNode(std::string specifier)
{
    type = N_VISIBILITY;
    if (specifier == "public")
        access = PUBLIC;
    else if (specifier == "private")
        access = PRIVATE;
    else if (specifier == "protected")
        access = PROTECTED;
}

AccessSpecifier VisibilityNode::getVisibility()
{
    return access;
}

Control VisibilityNode::resolve(State &state)
{
    if (children.size() != 1)
        return Control("expected one access-specified value");
    // this will be a method node or just an identifier
    if (dynamic_cast<IdentifierNode *>(children[0]))
    {
        IdentifierNode *identifier = (IdentifierNode *)children[0];
    }
}

std::string VisibilityNode::getVariable()
{
    if (dynamic_cast<VariableNode *>(children[0]))
    {
        VariableNode *variable = (VariableNode *)children[0];
        return variable->getVariable();
    }
    return "";
}

void VisibilityNode::makeStatic()
{
    isStatic = true;
}

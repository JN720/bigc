#include "TypeNode.h"
#include "IdentifierNode.h"

TypeNode::TypeNode(std::string &type)
{
    argType = type;
    type = N_IDENTIFIER;
}

std::string TypeNode::resolve(State &state)
{
    if (children.size() != 1)
        return "type assertion node should have 1 child";
    return "";
}

std::string TypeNode::getName()
{
    return ((IdentifierNode *)children[0])->getVariable();
}

std::string TypeNode::getArgType()
{
    return argType;
}

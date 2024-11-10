#include "TypeNode.h"

TypeNode::TypeNode(std::string type)
{
    argType = type;
    type = N_TYPE;
}

Control TypeNode::resolve(State &state)
{
    if (children.size() != 1)
        return Control("type assertion node should have 1 child");
    return Control(OK);
}

std::string TypeNode::getVariable()
{
    return ((VariableNode *)children[0])->getVariable();
}

std::string TypeNode::getArgType()
{
    return argType;
}

Node *TypeNode::copy()
{
    TypeNode *typeNode = new TypeNode(argType);
    typeNode->addChild(children[0]->copy());
    return typeNode;
}

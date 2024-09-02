#include "Node.h"
#include "Value.h"

Node::Node(Value value)
{
    this->value = value;
}

Node::Node()
{
}

Node::Node(std::vector<Node *> &children)
{
    this->children = children;
}

Value Node::getValue(const State &state)
{
    return value;
}

void Node::addChild(Node *child)
{
    children.push_back(child);
}

NodeType Node::getType() const
{
    return type;
}

std::vector<Node *> Node::getChildren() const
{
    return children;
}

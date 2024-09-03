#include "Node.h"
#include "Value.h"
#include "Array.h"

Node::Node(Value value)
{
    this->value = value;
    type = N_LEAFVALUE;
}

Node::Node()
{
    type = N_LEAFVALUE;
}

Node::Node(std::vector<Node *> &children)
{
    this->children = children;
}

Value Node::getValue(const State &state)
{
    return value;
}

std::string Node::resolve(State &state)
{
    if (state.implements(value.getType(), "iterable"))
    {
        Array *arr = new Array();
        for (auto child : children)
        {
            std::string error = child->resolve(state);
            if (!error.empty())
                return error;
            arr->add(child->getValue(state));
        }
        value = Value(arr);
        return "";
    }
    else if (children.empty())
        return "";
    return "unresolvable value";
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

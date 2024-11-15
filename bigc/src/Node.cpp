#include "Node.h"

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

Control Node::resolve(State &state)
{
    // a leaf value with children is an array
    // other iterables can be constructed from this
    if (children.size())
    {
        Array<Value> *arr = new Array<Value>();
        for (auto child : children)
        {
            Control control = child->resolve(state);
            if (control.control())
            {
                value = child->getValue(state);
                return control;
            }
            if (control.error())
                return control.stack("resolving array elements:\n");
            arr->add(child->getValue(state));
        }
        value = Value(arr);
        return Control(OK);
    }
    // value should have already been set
    else if (children.empty())
        return Control(OK);
    return Control("unresolvable value");
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

bool Node::hasChildren() const
{
    return children.size();
}

Node *Node::copy()
{
    Wildcard val = value.getValue();
    if (Node **node = std::get_if<Node *>(&val))
        return new Node(Value((*node)->copy()));
    return new Node(value);
}

void Node::destroy(State *state)
{
    if (Allocated *ref = state->getAllocated(value))
        state->removeRef(ref);
    delete this;
}

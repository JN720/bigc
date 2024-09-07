#include "IndexNode.h"

IndexNode::IndexNode()
{
}

Value IndexNode::getValue(const State &state)
{
    return Value();
}

std::string IndexNode::resolve(State &state)
{
    if (children.size() != 2)
        return "invalid number of arguments for indexing";
    for (auto child : children)
    {
        std::string error = child->resolve(state);
        if (!error.empty())
            return error;
    }
    Wildcard val = children[0]->getValue(state).getValue();
    if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
    {
        auto result = (*x)->get(children[1]->getValue(state));
        if (!result.ok())
            return result.getError();
        value = result.getValue();
        return "";
    }
    return "tried to index non-iterable";
}

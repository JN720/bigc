#include "IndexNode.h"

IndexNode::IndexNode()
{
    type = N_INDEX;
}

std::string IndexNode::resolve(State &state)
{
    if (children.size() != 2)
        return "invalid number of arguments for indexing";
    // the first child is the iterable
    // the second child is the index
    for (auto child : children)
    {
        std::string error = child->resolve(state);
        if (!error.empty())
            return "resolving index:\n" + error;
    }
    Wildcard val = children[0]->getValue(state).getValue();
    if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
    {
        Value index = children[1]->getValue(state);
        Result<Value> result = (*x)->get(&index);
        if (!result.ok())
            return "indexing iterable:\n" + result.getError();
        value = result.getValue();
        return "";
    }
    return "tried to index non-iterable";
}

#include "IndexNode.h"

IndexNode::IndexNode()
{
    type = N_INDEX;
}

Control IndexNode::resolve(State &state)
{
    if (children.size() != 2)
        return Control("invalid number of arguments for indexing");
    // the first child is the iterable
    // the second child is the index
    for (auto child : children)
    {
        Control control = child->resolve(state);
        if (control.control())
        {
            value = child->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("resolving index:\n");
    }
    Wildcard val = children[0]->getValue(state).getValue();
    if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
    {
        Value index = children[1]->getValue(state);
        Result<Value> result = (*x)->get(&index);
        if (!result.ok())
            return "indexing iterable:\n" + result.getError();
        value = result.getValue();
        return Control(OK);
    }
    return Control("tried to index non-iterable");
}

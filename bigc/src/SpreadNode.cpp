#include "SpreadNode.h"
#include "Spread.h"

SpreadNode::SpreadNode()
{
    type = N_SPREAD;
}

Control SpreadNode::resolve(State &state)
{
    if (children.size() != 1)
        return Control("expected one iterable to spread");
    Control control = children[0]->resolve(state);
    if (control.control())
    {
        value = children[0]->getValue(state);
        return control;
    }
    if (control.error())
        return control.stack("during spread:\n");
    value = children[0]->getValue(state);
    Wildcard val = value.getValue();
    // TODO: handle objects that implement Iterable
    if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
    {
        Spread<Value> *spread = new Spread<Value>();
        Control control = spread->applyIterable(*x);
        if (!control.ok())
            return control.stack("during spread:\n");
        value = Value(spread);
        return Control(OK);
    }
    else
        return Control("cannot spread " + value.getType());
}
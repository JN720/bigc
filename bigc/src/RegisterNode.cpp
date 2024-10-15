#include "RegisterNode.h"

RegisterNode::RegisterNode(std::string name)
{
    type = N_REGISTER;
    this->name = name;
}

Control RegisterNode::resolve(State &state)
{
    // register one value
    if (children.size() != 1)
        return Control("register node expects 1 child");
    Control control = children[0]->resolve(state);
    if (!control.ok())
        return control.stack("resolving value to register:\n");
    state.registerVariable(name, children[0]->getValue(state));
    return Control(OK);
}
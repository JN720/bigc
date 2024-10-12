#include "InterfaceNode.h"
#include "IdentifierNode.h"
#include "TypeNode.h"

InterfaceNode::InterfaceNode()
{
    type = N_INTERFACE;
}

Control InterfaceNode::resolve(State &state)
{
    // the children for this should be identifiers of methods or type nodes containing the method type
    interface = new Interface();
    for (Node *child : children)
    {
        if (IdentifierNode *id = dynamic_cast<IdentifierNode *>(child))
        {
            // add the method to the interface
            interface->addMethod(id->getVariable(), "fn");
        }
        else if (TypeNode *type = dynamic_cast<TypeNode *>(child))
        {
            // include the type of the argument
            interface->addMethod(id->getVariable(), type->getArgType());
        }
    }
    value = Value(this);
    return Control(OK);
}

Interface *InterfaceNode::getInterface()
{
    return interface;
}

#include "ClassNode.h"
#include "VisibilityNode.h"
#include "IdentifierNode.h"
#include "ClassDefinition.h"

ClassNode::ClassNode()
{
    type = N_CLASS;
}

ClassDefinition *ClassNode::getClassDefinition()
{
    return definition;
}

// every child should be a VisibilityNode
Control ClassNode::resolve(State &state)
{
    // the first children are interfaces and parent classes
    // the last child is the class sequence

    // start with a blank class
    definition = new ClassDefinition();
    Wildcard val;
    for (int i = 0; i < children.size() - 1; i++)
    {
        if (dynamic_cast<IdentifierNode *>(children[i]))
        {
            IdentifierNode *identifier = (IdentifierNode *)children[i];
            Control control = identifier->resolve(state);
            if (control.control())
            {
                value = identifier->getValue(state);
                return control;
            }
            if (control.error())
                return control.stack("during class definition:\n");
            val = identifier->getValue(state).getValue();
            Node **x;
            x = std::get_if<Node *>(&val);
            if (!x)
                return Control("invalid value for parent class/interface");
            // parent class
            if (dynamic_cast<ClassNode *>(*x))
            {
            }
            // interface
            else if (dynamic_cast<ClassNode *>(*x))
            {
            }
            else
                return Control("expected a class or interface");
        }
        else
            return Control("expected an identifier for parent classes and interfaces");
    }
    for (auto child : children)
    {
        Control control = child->resolve(state);
        if (control.control())
        {
            value = child->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("during class definition:\n");
    }
    for (auto child : children)
    {
        if (dynamic_cast<VisibilityNode *>(child))
        {
            VisibilityNode *visibility = (VisibilityNode *)child;
        }
    }
}

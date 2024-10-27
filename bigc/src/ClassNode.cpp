#include "ClassNode.h"
#include "VisibilityNode.h"
#include "IdentifierNode.h"
#include "ClassDefinition.h"
#include "InterfaceNode.h"
#include "FunctionNode.h"

ClassNode::ClassNode()
{
    type = N_CLASS;
}

ClassNode::ClassNode(ClassDefinition *definition)
{
    this->definition = definition;
    type = N_CLASS;
}

ClassNode::ClassNode(ClassDefinitionInterface *definition)
{
    this->definition = (ClassDefinition *)definition;
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
            // ensure the interface r parent class exists
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
            if (ClassNode *parentDef = dynamic_cast<ClassNode *>(*x))
            {
                definition->applyParent(parentDef->getClassDefinition());
            }
            // interface
            else if (dynamic_cast<InterfaceNode *>(*x))
            {
                InterfaceNode *interfaceDef = (InterfaceNode *)(*x);
                definition->applyInterface(interfaceDef->getInterface());
            }
            else
                return Control("expected a class or interface");
        }
        else
            return Control("expected an identifier for parent classes and interfaces");
    }
    // class sequence
    // manually evaluate the sequence's children
    for (int index = 0; index < children.back()->getChildren().size(); index++)
    {
        Node *child = children.back()->getChildren()[index];
        if (VisibilityNode *visibility = dynamic_cast<VisibilityNode *>(child))
        {
            // if the visibility node has two children, the second is the default value
            // therefore, we resolve the second child first
            if (!visibility->getIsMethod() && visibility->getChildren().size())
            {
                Control control = visibility->getChildren()[0]->resolve(state);
                if (!control.ok())
                    return control.stack("during declaration " + std::to_string(index + 1) + " in class definition:\n");
            }
            Control control = visibility->applyToDefinition(definition, state);
            if (!control.ok())
                return control.stack("during declaration " + std::to_string(index + 1) + " in class definition:\n");
        }
        else
            return Control("expected a visibility node");
    }
    value = Value(this);
    return Control(OK);
}
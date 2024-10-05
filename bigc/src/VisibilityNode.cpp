#include "VisibilityNode.h"
#include "IdentifierNode.h"
#include "FunctionNode.h"
#include "Value.h"

VisibilityNode::VisibilityNode()
{
    type = N_VISIBILITY;
    access = PUBLIC;
    isStatic = false;
    isMethod = false;
}

VisibilityNode::VisibilityNode(AccessSpecifier specifier)
{
    type = N_VISIBILITY;
    access = specifier;
    isStatic = false;
    isMethod = false;
}

VisibilityNode::VisibilityNode(std::string specifier)
{
    type = N_VISIBILITY;
    if (specifier == "public")
        access = PUBLIC;
    else if (specifier == "private")
        access = PRIVATE;
    else if (specifier == "protected")
        access = PROTECTED;
}

AccessSpecifier VisibilityNode::getVisibility()
{
    return access;
}

Control VisibilityNode::resolve(State &state)
{
    if (children.size() != 1)
        return Control("expected one access-specified value");
    // this will be a method node or just an identifier
    if (isMethod)
    {
        if (dynamic_cast<FunctionNode *>(children[0]))
        {
            Control control = children[0]->resolve(state);
            if (control.control())
            {
                value = children[0]->getValue(state);
                return control;
            }
            if (control.error())
                return control.stack("during method declaration:\n");
            value = children[0]->getValue(state);
        }
        else
            return Control("expected a function for method declaration");
    }
    else
    {
        if (dynamic_cast<IdentifierNode *>(children[0]))
        {
            IdentifierNode *identifier = (IdentifierNode *)children[0];
        }
        else
            return Control("expected an identifier for the attribute");
    }
}

std::string VisibilityNode::getVariable()
{
    if (dynamic_cast<VariableNode *>(children[0]))
    {
        VariableNode *variable = (VariableNode *)children[0];
        return variable->getVariable();
    }
    return "";
}

void VisibilityNode::makeStatic()
{
    isStatic = true;
}

void VisibilityNode::makeMethod()
{
    isMethod = true;
}

bool VisibilityNode::getIsStatic()
{
    return isStatic;
}

void VisibilityNode::applyToDefinition(ClassDefinition *definition)
{
    if (isMethod)
    {
        definition->addMethod(getVariable(), children[0], isStatic);
    }
    else
    {
        definition->addAttribute(variable, access, isStatic);
    }
}
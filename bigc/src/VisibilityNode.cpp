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
    isStatic = false;
    isMethod = false;
}

AccessSpecifier VisibilityNode::getVisibility()
{
    return access;
}

Control VisibilityNode::resolve(State &state)
{
    if (children.size() != 1 && children.size() != 2)
        return Control("expected one or two access-specified values");
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
        // if we have an attribute and there is a child, it is a default value
        if (children.size() == 1)
        {
            Control control = children[0]->resolve(state);
            if (!control.ok())
                return control.stack("during attribute declaration:\n");
        }
    }
    return Control(OK);
}

void VisibilityNode::makeStatic()
{
    isStatic = true;
}

void VisibilityNode::makeMethod()
{
    isMethod = true;
}

void VisibilityNode::setType(std::string type)
{
    this->attributeType = type;
}

bool VisibilityNode::getIsMethod()
{
    return isMethod;
}

bool VisibilityNode::getIsStatic()
{
    return isStatic;
}

void VisibilityNode::applyToDefinition(ClassDefinition *definition, const State &state)
{
    if (isMethod)
        definition->addMethod(variable, children[0], isStatic);
    else
    {
        if (isStatic)
        {
            if (children.size())
                definition->addStaticAttribute(variable, children[0]->getValue(state), access);
            else
                definition->addStaticAttribute(variable, Value(false), access);
        }
        else
        {
            definition->addAttribute(variable, access, isStatic);
            if (children.size())
                definition->setAttributeDefault(variable, children[0]->getValue(state));
            else
                definition->setAttributeDefault(variable, Value(false));
        }
    }
}
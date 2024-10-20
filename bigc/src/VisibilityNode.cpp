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

std::string VisibilityNode::getAttributeType()
{
    return attributeType;
}

bool VisibilityNode::getIsMethod()
{
    return isMethod;
}

bool VisibilityNode::getIsStatic()
{
    return isStatic;
}

Control VisibilityNode::applyToDefinition(ClassDefinition *definition, const State &state)
{
    // method overrides should only work for public and protected methods
    // TODO: they also require the same type signature
    if (isMethod)
    {
        if (definition->hasProperty(variable))
        {
            auto methods = definition->getMethods();
            Method method;
            if (methods.find(variable) == methods.end())
                methods = definition->getStaticMethods();
            if (methods.find(variable) == methods.end())
                return Control("method " + variable + " not found");
            method = methods.at(variable);
            if (method.access != PUBLIC && method.access != PROTECTED)
                return Control("method " + variable + " must be public or protected to override");
            // if (method.method->getType() != getAttributeType())
            //     return Control("method " + variable + " has wrong type signature");
        }
        definition->addMethod(variable, children[0], isStatic, access);
    }
    // the overriding rules for attributes are the same
    // they also require the same type
    else
    {
        if (definition->hasProperty(variable))
        {
            auto attributes = definition->getAttributes();
            Attribute attribute;
            if (attributes.find(variable) == attributes.end())
                attributes = definition->getStaticAttributes();
            if (attributes.find(variable) == attributes.end())
                return Control("attribute " + variable + " not found");
            attribute = attributes.at(variable);
            if (attribute.access != PUBLIC && attribute.access != PROTECTED)
                return Control("attribute " + variable + " must be public or protected to override");
            if (attribute.value.getType() != getAttributeType())
                return Control("attribute " + variable + " has wrong type signature");
        }
        definition->addAttribute(variable, access, isStatic, children.size() ? children[0]->getValue(state) : Value(false));
    }
    return Control(OK);
}

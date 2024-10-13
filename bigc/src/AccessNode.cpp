#include "AccessNode.h"
#include "Object.h"
#include "ClassNode.h"
#include "MethodNode.h"
#include "builtin.h"

AccessNode::AccessNode(const std::string &property)
{
    this->property = property;
    type = N_ACCESS;
}

Control AccessNode::resolve(State &state)
{
    // this should be an object
    if (children.size() != 1)
        return Control("invalid access node");
    Control control = children.front()->resolve(state);
    if (!control.ok())
        return control.stack("accessing property:\n");
    Wildcard val = children.front()->getValue(state).getValue();
    if (ClassNode **cls = (ClassNode **)std::get_if<Node *>(&val))
    {
        // this is like accessing on an instanced object but we check the static attributes and methods
        Result<Value> result = (*cls)->getClassDefinition()->getStaticAttribute(property);
        if (result.ok())
        {
            value = result.getValue();
            return Control(OK);
        }
        // else try to access the method
        else
        {
            Result<Node *> method = (*cls)->getClassDefinition()->getStaticMethod(property);
            if (method.ok())
            {
                value = Value(method.getValue());
                return Control(OK);
            }
            return Control(method.getError()).stack("accessing class property:\n");
        }
    }
    else if (VariableNode **var = (VariableNode **)std::get_if<Node *>(&val))
    {
        // try to access the attribute first
        Result<Value> result = (*var)->getValue(state);
        if (result.ok())
        {
            value = result.getValue();
            return Control(OK);
        }
    }
    else if (Object **obj = std::get_if<Object *>(&val))
    {
        // try to access the attribute first
        Result<Value> result = (*obj)->getProperty(property);
        if (result.ok())
        {
            value = result.getValue();
            return Control(OK);
        }
        // else try to access the method
        else
        {
            Result<Node *> method = (*obj)->getClass()->getClassMethod(property);
            if (method.ok())
            {
                value = Value(new MethodNode(method.getValue(), *obj));
                return Control(OK);
            }
            return Control(method.getError()).stack("accessing object property:\n");
        }
    }
    else
        return Control("tried to access property of non-object");
    return Control("failed to resolve access node");
}

Control AccessNode::setValue(State &state, Value value)
{
    if (children.empty())
        return Control("AccessNode has no child to access");

    Control control = children[0]->resolve(state);
    if (control.control())
    {
        value = children[0]->getValue(state);
        return control;
    }
    else if (control.error())
        return control.stack("Error resolving child for attribute access");

    Wildcard val = children[0]->getValue(state).getValue();
    if (ClassNode **cls = (ClassNode **)std::get_if<Node *>(&val))
    {
        (*cls)->getClassDefinition()->setStaticAttribute(property, value);
    }
    else if (VariableNode **var = (VariableNode **)std::get_if<Node *>(&val))
    {
        (*var)->setValue(state, value);
    }
    else if (Object **obj = std::get_if<Object *>(&val))
    {
        control = (*obj)->setProperty(property, value);
        if (control.error())
            return control.stack("Error setting object property");
    }
    else
    {
        return Control("Cannot set attribute on non-object value");
    }
    return Control(OK);
}
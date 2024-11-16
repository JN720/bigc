#include "AccessNode.h"
#include "Object.h"
#include "ClassNode.h"
#include "MethodNode.h"
#include "StaticMethodNode.h"
#include "builtin.h"
#include "LibraryNode.h"
#include <typeinfo>

AccessNode::AccessNode(const std::string &property)
{
    this->property = property;
    type = N_ACCESS;
}

Control AccessNode::resolve(State &state)
{
    if (children.size() != 1)
        return Control("invalid access node");
    Control control = children.front()->resolve(state);
    if (!control.ok())
        return control.stack("accessing property:\n");
    Wildcard val = children.front()->getValue(state).getValue();
    AccessType accessType = (dynamic_cast<VariableNode *>(children.front()) &&
                                 static_cast<VariableNode *>(children.front())->getVariable() == "this" ||
                             static_cast<VariableNode *>(children.front())->getVariable() == "static")
                                ? THIS
                                : OUTSIDE;

    if (dynamic_cast<VariableNode *>(children.front())->getVariable() == "super")
        accessType = SUPER;
    if (dynamic_cast<VariableNode *>(children.front())->getVariable() == "superstatic")
        accessType = SUPER;
    Node **node = std::get_if<Node *>(&val);
    if (node && *node)
    {
        if (ClassNode *cls = dynamic_cast<ClassNode *>(*node))
        {
            ClassDefinition *classDefinition = cls->getClassDefinition();
            if (classDefinition == nullptr)
                return Control("tried to access property of null");

            if (!(classDefinition->canAccess(property, accessType)))
                return Control("Cannot access " + property + ": access denied");

            Result<Value> classData = classDefinition->getStaticAttribute(property);

            if (classData.ok())
            {
                value = classData.getValue();
                return Control(OK);
            }
            else
            {
                Result<Node *> method = cls->getClassDefinition()->getStaticMethod(property);
                if (method.ok())
                {
                    value = Value(new StaticMethodNode(method.getValue(), cls, state));
                    return Control(OK);
                }
                return Control(method.getError()).stack("accessing class property:\n");
            }
        }
        else if (LibraryNode *lib = dynamic_cast<LibraryNode *>(*node))
        {
            Result<Value> result = lib->getRegistry()->getVariable(property);
            if (result.ok())
            {
                value = result.getValue();
                return Control(OK);
            }
        }
        else if (VariableNode *var = dynamic_cast<VariableNode *>(*node))
        {
            Result<Value> result = var->getValue(state);
            if (result.ok())
            {
                value = result.getValue();
                return Control(OK);
            }
        }
    }
    else if (Object **obj = std::get_if<Object *>(&val))
    {

        if (!*obj)
            return Control("tried to access property of null");
        ClassDefinition *classDefinition = static_cast<ClassDefinition *>((*obj)->getClass());
        if (!classDefinition)
            return Control("object does not have a class definition");
        if (!(classDefinition->canAccess(property, accessType)))
            return Control("Cannot access " + property + ": access denied");

        Result<Value> result = (*obj)->getProperty(property);
        if (result.ok())
        {
            value = result.getValue();
            return Control(OK);
        }
        else
        {
            // this should be function node-like
            Result<Node *> method = classDefinition->getClassMethod(property);
            if (method.ok())
            {
                Node *function = method.getValue();
                if (!dynamic_cast<MethodNode *>(function))
                    function = new MethodNode(function, *obj, state);
                else
                    ((MethodNode *)function)->setObject(*obj);
                value = Value(function);
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

    AccessType accessType = (dynamic_cast<VariableNode *>(children.front()) &&
                                 static_cast<VariableNode *>(children.front())->getVariable() == "this" ||
                             static_cast<VariableNode *>(children.front())->getVariable() == "static")
                                ? THIS
                                : OUTSIDE;

    if (dynamic_cast<VariableNode *>(children.front())->getVariable() == "super")
        accessType = SUPER;
    if (dynamic_cast<VariableNode *>(children.front())->getVariable() == "superstatic")
        accessType = SUPER;

    if (Node **node = std::get_if<Node *>(&val))
    {
        if (ClassNode *cls = dynamic_cast<ClassNode *>(*node))
        {
            ClassDefinition *classDefinition = cls->getClassDefinition();
            if (!classDefinition->canAccess(property, accessType))
                return Control("Cannot set " + property + ": access denied");
            classDefinition->setStaticAttribute(property, value);
        }
        else if (LibraryNode *lib = dynamic_cast<LibraryNode *>(*node))
        {
            return Control("Cannot set attribute on library");
        }
        else if (VariableNode *var = dynamic_cast<VariableNode *>(*node))
        {
            var->setValue(state, value);
        }
        else
        {
            return Control("Cannot set attribute on unknown Node type");
        }
    }
    else if (Object **obj = std::get_if<Object *>(&val))
    {
        ClassDefinition *classDefinition = static_cast<ClassDefinition *>((*obj)->getClass());
        if (!classDefinition->canAccess(property, accessType))
            return Control("Cannot set " + property + ": access denied");
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

#include "IndexNode.h"
#include "InterfaceNode.h"
#include "FunctionNode.h"
#include "ClassDefinition.h"

IndexNode::IndexNode()
{
    type = N_INDEX;
}

Control IndexNode::resolve(State &state)
{
    if (children.size() != 2)
        return Control("invalid number of arguments for indexing");
    // the first child is the iterable
    // the second child is the index
    for (auto child : children)
    {
        Control control = child->resolve(state);
        if (control.control())
        {
            value = child->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("resolving index:\n");
    }
    Wildcard val = children[0]->getValue(state).getValue();
    Value index = children[1]->getValue(state);
    if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
    {
        Result<Value> result = (*x)->get(&index);
        if (!result.ok())
            return "indexing iterable:\n" + result.getError();
        value = result.getValue();
        return Control(OK);
    }
    else if (Object **obj = std::get_if<Object *>(&val))
    {
        Result<Value> indexableResult = state.getVariable("Indexable");
        if (!indexableResult.ok())
            return Control("getting Indexable:\n" + indexableResult.getError());
        Wildcard val = indexableResult.getValue().getValue();
        if (Node **x = std::get_if<Node *>(&val))
        {
            if (InterfaceNode *indexableNode = dynamic_cast<InterfaceNode *>(*x))
            {
                Interface *indexable = indexableNode->getInterface();
                ClassDefinition *objClass = static_cast<ClassDefinition *>((*obj)->getClass());
                if (objClass->implements(indexable))
                {
                    Result<Node *> result = objClass->getClassMethod("index");
                    if (result.ok())
                    {
                        FunctionNode *function = (FunctionNode *)result.getValue();
                        if (!function)
                            return Control("invalid function");
                        std::vector<Node *> args({new Node(Value(index))});
                        Result<Value> output = function->execute(state, args);
                        if (!output.ok())
                            return Control("executing index:\n" + output.getError());
                        return Control(OK);
                    }
                    else
                        return Control("getting index method on object:\n" + result.getError());
                }
                else
                    return Control("object class does not implement Indexable");
            }
            else
                return Control("Indexable is not an interface");
        }
        return Control("indexable interface is not a node");
    }
    return Control("tried to index non-iterable");
}

Control IndexNode::setValue(State &state, Value value)
{
    Wildcard val = children[0]->getValue(state).getValue();
    if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
    {
        Value index = children[1]->getValue(state);
        Result<Value> result = (*x)->get(&index);
        // remove ref if there is an old value in the iterable
        if (result.ok())
        {
            Allocated *ref = state.getAllocated(result.getValue());
            if (ref)
                state.removeRef(ref);
        }
        // add ref for inserting into iterable
        if (Allocated *ref = state.getAllocated(value))
            state.addRef(ref);
        return (*x)->set(&index, value);
    }
    else if (Node **x = std::get_if<Node *>(&val))
    {
        if (VariableNode *var = dynamic_cast<VariableNode *>(*x))
            return var->setValue(state, value);
        return Control("tried to assign to non-variable");
    }
    return Control("tried to assign to non-variable");
}
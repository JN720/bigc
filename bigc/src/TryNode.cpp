#include "TryNode.h"
#include "builtin.h"
#include "NullObject.h"
#include "ClassNode.h"
#include "ClassDefinition.h"

TryNode::TryNode()
{
    type = N_TRY;
}

Control TryNode::resolve(State &state)
{
    if (children.size() != 1)
        return Control("try node should have 1 child");
    Control control = children[0]->resolve(state);
    if (control.control())
    {
        value = children[0]->getValue(state);
        return control;
    }
    if (control.error())
    {
        Result<Value> result = state.getVariable("Exception");
        if (!result.ok())
            return Control("Exception not found");
        Wildcard val = result.getValue().getValue();
        if (Node **exceptionNode = std::get_if<Node *>(&val))
        {
            if (ClassNode *cls = dynamic_cast<ClassNode *>(*exceptionNode))
            {
                ClassDefinition *classDefinition = cls->getClassDefinition();
                if (!classDefinition)
                    return Control("Exception class definition does not exist");
                Node *message = new Node(Value(new std::string(control.getError())));
                std::vector<Node *> constructionArgs({message});
                Result<Value> result = classDefinition->construct(&state, constructionArgs);
                if (!result.ok())
                    return Control("Exception construction failed");
                value = result.getValue();
                return Control(OK);
            }
            else
                return Control("Exception is not a class");
        }
        else
            return Control("Exception is not a node");
    }

    return Control(OK);
}

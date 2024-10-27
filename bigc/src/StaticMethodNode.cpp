#include "StaticMethodNode.h"
#include "AccessNode.h"
#include "TypeNode.h"
#include "VariableNode.h"

StaticMethodNode::StaticMethodNode(Node *method, ClassNode *cls)
{
    this->cls = cls;
    children = method->getChildren();
}

// we need to pass the class definition to the method so it can access static attributes
Result<Value> StaticMethodNode::execute(State &state, std::vector<Node *> &args)
{
    // function call scopes are closed
    StateFrame *frame = state.pushFrame(true);
    frame->setVariable("static", Value((Node *)cls));

    int curVal = 0;
    int curArg = 0;
    // place arguments until we hit the sequence
    while (curArg < children.size() - 1 && curVal < args.size())
    {
        Node *arg = args[curVal];
        Node *child = children[curArg];
        // there should be one more child than args because the last is the end
        // we have already resolved it so get the value
        Value val = args[curVal]->getValue(state);
        if (dynamic_cast<VariableNode *>(child))
        {
            // get the name of the function arg and set its value to val
            frame->setVariable(((VariableNode *)child)->getVariable(), val);
            curVal++;
            curArg++;
        }
        else if (dynamic_cast<TypeNode *>(child))
        {
            TypeNode *typed = (TypeNode *)arg;
            if (typed->getArgType() != val.getType())
            {
                state.popFrame();
                return Result<Value>("type assertion failed");
            }
            frame->setVariable(typed->getVariable(), val);
            curVal++;
            curArg++;
        }
        /*
        else if (dynamic_cast<SpreadNode *>(child))
        {
            SpreadNode *spread = (SpreadNode *)arg;
            if (dynamic_cast<IdentifierNode *>(child)) {
                spread->get
            }
            // go to the next value but stay on the same arg
            curVal++;
        }*/
        else
            return Result<Value>("invalid node in function arguments");
    }
    // resolve the sequence node
    Control control = children.back()->resolve(state);
    if (control.control())
    {
        state.popFrame();
        // return from a function
        if (control.getSignal() == RETURN)
            return Result<Value>(children.back()->getValue(state));
        return Result<Value>("unexpected control signal");
    }
    if (control.error())
    {
        state.popFrame();
        return Result<Value>(control.stack("during function execution:\n"));
    }
    Value result = children.back()->getValue(state);
    state.popFrame();
    return Result<Value>(result);
}

Control StaticMethodNode::resolve(State &state)
{
    if (children.size() < 1)
        return Control("static method must have sequence");
    value = Value(this);
    return Control(OK);
}

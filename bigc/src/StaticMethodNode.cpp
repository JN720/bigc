#include "StaticMethodNode.h"
#include "AccessNode.h"
#include "TypeNode.h"
#include "VariableNode.h"

StaticMethodNode::StaticMethodNode(Node *method, ClassNode *cls, State &state)
{
    this->cls = cls;
    children = method->getChildren();
    for (auto child : children)
    {
        state.addRef(child);
    }
    state.addRef((Node *)cls);
}

// we need to pass the class definition to the method so it can access static attributes
Result<Value> StaticMethodNode::execute(State &state, std::vector<Node *> &args)
{
    // function call scopes are closed
    StateFrame *frame = state.pushFrame(true);
    state.setVariable("static", Value((Node *)cls));
    Result<std::vector<Value>> result = valuifyArgs(args, state);
    if (!result.ok())
    {
        state.popFrame();
        return Result<Value>("aligning arguments:\n" + result.getError());
    }
    std::vector<Value> argValues = result.getValue();

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
    Value val = children.back()->getValue(state);
    if (Allocated *ref = state.getAllocated(val))
        state.addRef(ref);
    state.popFrame();
    return Result<Value>(val);
}

Control StaticMethodNode::resolve(State &state)
{
    if (children.size() < 1)
        return Control("static method must have sequence");
    value = Value(this);
    return Control(OK);
}

void StaticMethodNode::destroy(State *state)
{
    ((Node *)cls)->destroy(state);
    Node::destroy(state);
}

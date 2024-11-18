#include "FunctionNode.h"
#include "TypeNode.h"
#include "SpreadNode.h"
#include "SequenceNode.h"
#include "Object.h"
#include "ClassNode.h"
#include "ArgumentIterator.h"

FunctionNode::FunctionNode()
{
    type = N_LEAFVALUE;
}

Control FunctionNode::resolve(State &state)
{
    if (children.size() < 1)
        return Control("function must have sequence");
    state.addRef(this);
    value = Value(this);
    return Control(OK);
}

Result<Value> FunctionNode::execute(State &state, std::vector<Node *> &args)
{
    // each arg should be a variable node or type node
    // the final one can also be a spread node
    // create the frame for the arguments we pass in
    StateFrame *frame = state.pushFrame(true);
    state.setVariable("thisfn", Value(this));
    // place arguments until we hit the sequence
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
        // return from a function
        state.popFrame();
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

// this is a copy of execute but includes a this in the function call scope
Result<Value> FunctionNode::executeInstanced(Object *obj, State *state, std::vector<Node *> &args)
{
    // each arg should be a variable node or type node
    // the final one can also be a spread node
    // create the frame for the arguments we pass in
    StateFrame *frame = state->pushFrame(true);
    // add this, static, and super
    state->setVariable("this", Value(obj));
    if (ClassDefinitionInterface *objClass = obj->getClass())
        state->setVariable("static", Value((Node *)new ClassNode(objClass)));
    Object *superObject = obj->getSuper();
    if (superObject)
        state->setVariable("super", Value(superObject));
    ClassDefinition *superClass = static_cast<ClassDefinition *>(obj->getClass()->getParent());
    if (superClass)
        state->setVariable("superstatic", Value((Node *)new ClassNode(superClass)));
    Result<std::vector<Value>> result = valuifyArgs(args, *state);
    if (!result.ok())
    {
        state->popFrame();
        return Result<Value>("aligning arguments:\n" + result.getError());
    }
    std::vector<Value> argValues = result.getValue();

    // resolve the sequence node
    Control control = children.back()->resolve(*state);

    if (control.control())
    {
        // return from a function
        state->popFrame();
        if (control.getSignal() == RETURN)
            return Result<Value>(children.back()->getValue(*state));
        return Result<Value>("unexpected control signal");
    }
    if (control.error())
    {
        state->popFrame();
        return Result<Value>(control.stack("during function execution:\n"));
    }
    Value val = children.back()->getValue(*state);
    if (Allocated *ref = state->getAllocated(val))
        state->addRef(ref);
    state->popFrame();
    return Result<Value>(val);
}

Result<std::vector<Value>> FunctionNode::valuifyArgs(std::vector<Node *> &args, State &state)
{
    std::vector<Value> argValues;
    ArgumentIterator argIterator(args);
    // the last child is the sequence
    int curChild = 0;
    while (curChild < children.size() - 1)
    {
        Result<Value> argResult = argIterator.next(&state);
        Node *child = children[curChild];
        // no more arguments passed
        if (argResult.getSignal() == BREAK)
        {
            // we expect to be fulfilling the final function argument
            // this is the only success case
            if (curChild != children.size() - 2)
                return Result<std::vector<Value>>("missing arguments");
            return Result<std::vector<Value>>(argValues);
        }
        if (!argResult.ok())
            return Result<std::vector<Value>>("aligning arguments:\n" + argResult.getError());
        Value val = argResult.getValue();
        // check type matching
        if (dynamic_cast<TypeNode *>(child))
        {
            TypeNode *typed = (TypeNode *)child;
            if (typed->getArgType() != val.getType())
            {
                return Result<std::vector<Value>>("type assertion failed: expected " + typed->getArgType() + " but got " + val.getType() + " instead");
            }
            state.setVariable(typed->getVariable(), val);
            curChild++;
        }
        // for dynamic types
        else if (dynamic_cast<VariableNode *>(child))
        {
            // get the name of the function arg and set its value to val
            state.setVariable(((VariableNode *)child)->getVariable(), val);
            curChild++;
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
        {
            return Result<std::vector<Value>>("invalid node in function arguments");
        }
    }
    if (argIterator.next(&state).getSignal() == BREAK)
        return Result<std::vector<Value>>(argValues);
    return Result<std::vector<Value>>("too many arguments given");
}

Control FunctionNode::resolveArguments(State &state, std::vector<Node *> &args)
{
    // resolve arguments until we hit the sequence
    for (Node *arg : args)
    {
        Control control = arg->resolve(state);
        if (control.control())
            return control;
        if (control.error())
            return control.stack("resolving arguments:\n");
    }

    return Control(OK);
}

std::string FunctionNode::getFunctionSignature()
{
    std::string signature = "fn";
    if (children.size() > 1)
    {
        signature += '(';
        for (int i = 0; i < children.size() - 1; ++i)
        {
            if (TypeNode *argType = dynamic_cast<TypeNode *>(children[i]))
                signature += argType->getArgType();
            else
                signature += "dyn";
            if (i != children.size() - 2)
                signature += ',';
        }
        signature += ')';
    }
    return signature;
}

Node *FunctionNode::copy()
{
    FunctionNode *fn = new FunctionNode();
    for (auto child : children)
    {
        fn->addChild(child->copy());
    }
    return fn;
}

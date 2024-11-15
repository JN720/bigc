#include "FunctionNode.h"
#include "TypeNode.h"
#include "SpreadNode.h"
#include "SequenceNode.h"
#include "Object.h"
#include "ClassNode.h"

FunctionNode::FunctionNode()
{
    type = N_LEAFVALUE;
}

Control FunctionNode::resolve(State &state)
{
    if (children.size() < 1)
        return Control("function must have sequence");
    value = Value(this);
    return Control(OK);
}

Result<Value> FunctionNode::execute(State &state, std::vector<Node *> &args)
{
    // each arg should be a variable node or type node
    // the final one can also be a spread node
    // create the frame for the arguments we pass in
    StateFrame *frame = state.pushFrame(true);
    frame->setVariable("thisfn", Value(this));
    // take each arg, do type assertions, and add to the frame
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
        if (dynamic_cast<TypeNode *>(child))
        {
            TypeNode *typed = (TypeNode *)child;
            if (typed->getArgType() != val.getType())
            {
                state.popFrame();
                return Result<Value>("type assertion failed: " + typed->getArgType() + " expected but got " + val.getType());
            }
            frame->setVariable(typed->getVariable(), val);
            curVal++;
            curArg++;
        }
        else if (dynamic_cast<VariableNode *>(child))
        {
            // get the name of the function arg and set its value to val
            frame->setVariable(((VariableNode *)child)->getVariable(), val);
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
        {
            state.popFrame();
            return Result<Value>("invalid node in function arguments");
        }
    }
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
    Value result = children.back()->getValue(state);
    state.popFrame();
    return Result<Value>(result);
}

// this is a copy of execute but includes a this in the function call scope
Result<Value> FunctionNode::executeInstanced(Object *obj, State *state, std::vector<Node *> &args)
{
    // each arg should be a variable node or type node
    // the final one can also be a spread node
    // create the frame for the arguments we pass in
    StateFrame *frame = state->pushFrame(true);
    // add this, static, and super
    frame->setVariable("this", Value(obj));
    if (ClassDefinitionInterface *objClass = obj->getClass())
        frame->setVariable("static", Value((Node *)new ClassNode(objClass)));
    Object *superObject = obj->getSuper();
    if (superObject)
        frame->setVariable("super", Value(superObject));
    ClassDefinition *superClass = static_cast<ClassDefinition *>(obj->getClass()->getParent());
    if (superClass)
        frame->setVariable("superstatic", Value((Node *)new ClassNode(superClass)));

    // take each arg, do type assertions, and add to the frame
    int curVal = 0;
    int curArg = 0;
    // place arguments until we hit the sequence
    while (curArg < children.size() - 1 && curVal < args.size())
    {
        Node *arg = args[curVal];
        std::cout << children.size() << '\n';
        Node *child = children[curArg];
        // there should be one more child than args because the last is the end
        // we have already resolved it so get the value
        std::cout << state << '\n';
        Value val = args[curVal]->getValue(*state);
        if (dynamic_cast<TypeNode *>(child))
        {
            TypeNode *typed = (TypeNode *)child;
            if (typed->getArgType() != val.getType())
            {
                state->popFrame();
                return Result<Value>("type assertion failed: expected " + typed->getArgType() + " but got " + val.getType() + " instead");
            }
            frame->setVariable(typed->getVariable(), val);
            curVal++;
            curArg++;
        }
        else if (dynamic_cast<VariableNode *>(child))
        {
            // get the name of the function arg and set its value to val
            frame->setVariable(((VariableNode *)child)->getVariable(), val);
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
        {
            state->popFrame();
            return Result<Value>("invalid node in function arguments");
        }
    }

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
    Value result = children.back()->getValue(*state);
    state->popFrame();
    return Result<Value>(result);
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
    std::cout << "signature is " << signature << "\n\n";
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

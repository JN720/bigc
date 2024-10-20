#include "IterateNode.h"
#include "SequenceNode.h"

IterateNode::IterateNode() : VariableNode()
{
    type = N_ITERATE;
    iterator = nullptr;
    currentState = 0;
}

IterateNode::IterateNode(std::string var) : VariableNode(var)
{
    type = N_ITERATE;
    iterator = nullptr;
    currentState = 0;
}

Control IterateNode::resolve(State &state)
{
    if (children.size() != 2)
        return Control("iterate requires an iterable and a sequence");

    // Resolve the first child to get the iterable
    Control control = children[0]->resolve(state);
    if (control.error())
        return control.stack("evaluating iterable in iterate:\n");

    Value iterableValue = children[0]->getValue(state);
    Wildcard iterableWildcard = iterableValue.getValue();

    // Check if the result is an Iterable
    Iterable<Value> **iterablePtr = std::get_if<Iterable<Value> *>(&iterableWildcard);
    if (!iterablePtr)
        return Control("first child of iterate must result in an iterable");

    iterator = *iterablePtr;
    currentState = 0;

    // Set the initial value to false (in case no iteration occurs)
    value = Value(false);

    // push a non-enclosing frame
    state.pushFrame(false);
    while (true)
    {
        Result<IteratorResult<Value>> nextResult = iterator->next(currentState);
        if (!nextResult.ok())
        {
            if (nextResult.getSignal() == BREAK)
            {
                // End of iteration
                return Control(OK);
            }
            return Control(nextResult.getError());
        }

        IteratorResult<Value> iterResult = nextResult.getValue();

        // Set the variable to the current item
        state.setVariable(variable, iterResult.value);

        // Update the current state
        currentState = iterResult.nextState;

        // Execute the sequence (second child)
        control = children[1]->resolve(state);
        if (control.control())
        {
            if (control.getSignal() == BREAK)
            {
                // Handle break
                if (children[1]->hasChildren())
                    value = children[1]->getValue(state);
                return Control(OK);
            }
            if (control.getSignal() == CONTINUE)
            {
                // Handle continue
                if (children[1]->hasChildren())
                    value = children[1]->getValue(state);
                continue;
            }
            // Return propagates upward
            value = children[1]->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("during iteration:\n");

        // Update the value with the result of the sequence
        value = children[1]->getValue(state);
    }

    return Control("failed to handle iteration");
}

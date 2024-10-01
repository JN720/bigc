#include "LoopNode.h"
#include "builtin.h"

LoopNode::LoopNode()
{
    type = N_LOOP;
}

Control LoopNode::resolve(State &state)
{
    if (children.size() != 2)
        return Control("loop requires a condition and a sequence");
    // if the loop does not execute let the value be false
    value = Value(false);
    while (true)
    {
        // get condition
        Control control = children[0]->resolve(state);
        if (control.control())
        {
            value = children[0]->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("evaluating loop condition:\n");
        Value conditionVal = children[0]->getValue(state);
        Wildcard condition = conditionVal.getValue();
        bool isTrue = true;
        // check condition value
        if (conditionVal.getType() != "nil")
        {
            if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&condition))
            {
                Result<int> result = (*x)->len();
                if (!result.ok())
                    return Control(result.getError());
                condition = result.getValue();
            }

            if (bool *x = std::get_if<bool>(&condition))
                isTrue = *x;
            else if (int *x = std::get_if<int>(&condition))
                isTrue = *x != 0;
            else if (long *x = std::get_if<long>(&condition))
                isTrue = *x != 0;
            else if (char *x = std::get_if<char>(&condition))
                isTrue = *x != 0;
            else if (float *x = std::get_if<float>(&condition))
                isTrue = *x != 0;
            else if (double *x = std::get_if<double>(&condition))
                isTrue = *x != 0;
            else if (std::string **x = std::get_if<std::string *>(&condition))
                isTrue = **x != "";
        }
        else
            isTrue = false;
        if (!isTrue)
            return Control(OK);
        // if the condition is satisfied then execute the sequence
        control = children[1]->resolve(state);
        if (control.control())
        {
            // handle break and continue
            if (control.getSignal() == BREAK)
            {
                if (children[1]->hasChildren())
                    value = children[1]->getValue(state);
                return Control(OK);
            }
            if (control.getSignal() == CONTINUE)
            {
                if (children[1]->hasChildren())
                    value = children[1]->getValue(state);
                continue;
            }
            // return propagates upward
            value = children[1]->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("during loop:\n");
        value = children[1]->getValue(state);
    }

    return Control("failed to handle loop");
}

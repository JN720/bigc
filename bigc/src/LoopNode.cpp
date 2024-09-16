#include "LoopNode.h"

LoopNode::LoopNode()
{
    type = N_LOOP;
}

std::string LoopNode::resolve(State &state)
{
    if (children.size() != 2)
        return "loop requires a condition and a sequence";
    // if the loop does not execute let the value be false
    value = Value(false);
    while (true)
    {
        // get condition
        std::string error = children[0]->resolve(state);
        if (!error.empty())
            return error;
        Value conditionVal = children[0]->getValue(state);
        Wildcard condition = conditionVal.getValue();
        bool isTrue = true;
        // check condition value
        if (conditionVal.getType() != "nil")
        {
            if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&condition))
            {
                auto result = (*x)->len();
                if (!result.ok())
                    return result.getError();
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
            return "";
        // if the condition is satisfied then executre sequence
        error = children[1]->resolve(state);
        if (!error.empty())
            return "during loop:\n" + error;
        value = children[1]->getValue(state);
    }

    return "failed to handle loop";
}

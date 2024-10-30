#include "BranchNode.h"
#include "NullObject.h"

BranchNode::BranchNode()
{
    type = N_BRANCH;
}

Control BranchNode::resolve(State &state)
{
    if (children.size() < 2)
        return Control("branch requires a condition and sequence");
    Control control = children[0]->resolve(state);
    if (control.control())
    {
        value = children[0]->getValue(state);
        return control;
    }
    if (control.error())
        return control.stack("in branch condition:\n");
    Value conditionVal = children[0]->getValue(state);
    Wildcard condition = conditionVal.getValue();
    bool isTrue = false;
    if (conditionVal.getType() != "nil")
    {
        if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&condition))
        {
            Result<int> result = (*x)->len();
            if (!result.ok())
                return "in branch condition:\n" + result.getError();
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
    if (isTrue)
    {
        // ignore the else portion
        Control control = children[1]->resolve(state);
        if (control.control())
        {
            value = children[1]->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("in branch sequence:\n");
        value = children[1]->getValue(state);
        return Control(OK);
    }
    else
    {
        // only the else condition
        if (children.size() == 3)
        {
            children[2]->resolve(state);
            value = children[2]->getValue(state);
            return Control(OK);
        }
        // if there is no else statement it is false
        else
        {
            value = Value(new NullObject());
            return Control(OK);
        }
    }

    return Control("failed to handle branch");
}

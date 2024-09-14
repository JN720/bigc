#include "BranchNode.h"

BranchNode::BranchNode()
{
    type = N_BRANCH;
}

std::string BranchNode::resolve(State &state)
{
    if (children.size() < 2)
        return "branch requires a condition and sequence";
    std::string error = children[0]->resolve(state);
    if (!error.empty())
        return "in branch condition:\n" + error;
    Value conditionVal = children[0]->getValue(state);
    Wildcard condition = conditionVal.getValue();
    bool isTrue = false;
    if (conditionVal.getType() != "nil")
    {
        if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&condition))
        {
            auto result = (*x)->len();
            if (!result.ok())
                return "in branch condition:\n" + result.getError();
            condition = result.getValue().getValue();
        }

        if (bool **x = std::get_if<bool *>(&condition))
            isTrue = **x;
        else if (int **x = std::get_if<int *>(&condition))
            isTrue = **x != 0;
        else if (long **x = std::get_if<long *>(&condition))
            isTrue = **x != 0;
        else if (char **x = std::get_if<char *>(&condition))
            isTrue = **x != 0;
        else if (float **x = std::get_if<float *>(&condition))
            isTrue = **x != 0;
        else if (double **x = std::get_if<double *>(&condition))
            isTrue = **x != 0;
        else if (std::string **x = std::get_if<std::string *>(&condition))
            isTrue = **x != "";
    }
    if (isTrue)
    {
        // ignore the else portion
        std::string error = children[1]->resolve(state);
        if (!error.empty())
            return "in branch sequence:\n" + error;
        value = children[1]->getValue(state);
        return "";
    }
    else
    {
        // only the else condition
        if (children.size() == 3)
        {
            children[2]->resolve(state);
            value = children[2]->getValue(state);
            return "";
        }
        // if there is no else statement it is false
        else
        {
            value = Value(new bool(false));
            return "";
        }
    }

    return "failed to handle branch";
}

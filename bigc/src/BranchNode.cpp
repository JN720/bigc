#include "BranchNode.h"
#include "UnbranchNode.h"

BranchNode::BranchNode()
{
    type = N_BRANCH;
}

std::string BranchNode::resolve(State &state)
{
    if (children.size() < 1)
        return "branch requires a condition";
    std::string error = children[0]->resolve(state);
    if (!error.empty())
        return error;
    Value conditionVal = children[0]->getValue(state);
    Wildcard condition = conditionVal.getValue();
    bool isTrue = false;
    if (conditionVal.getType() != "nil")
    {
        if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&condition))
        {
            auto result = (*x)->len();
            if (!result.ok())
                return result.getError();
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
        // if we have an else
        if (dynamic_cast<UnbranchNode *>(children.back()))
        {
            for (int i = 1; i < children.size() - 1; i++)
            {
                std::string error = children[i]->resolve(state);
                if (!error.empty())
                    return error;
            }
            value = children[children.size() - 2]->getValue(state);
            return "";
        }
        else
        {
            for (int i = 1; i < children.size(); i++)
            {
                std::string error = children[i]->resolve(state);
                if (!error.empty())
                    return error;
            }
            value = children.back()->getValue(state);
            return "";
        }
    }
    else
    {
        // only the else condition
        if (dynamic_cast<UnbranchNode *>(children.back()))
        {
            children.back()->resolve(state);
            value = children.back()->getValue(state);
            return "";
        }
    }

    return "failed to handle branch";
}

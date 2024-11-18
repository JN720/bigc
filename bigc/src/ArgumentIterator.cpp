#include "ArgumentIterator.h"
#include "Spread.h"

// the nodes should be resolved
ArgumentIterator::ArgumentIterator(std::vector<Node *> &args)
{
    this->args = args;
    this->index = 0;
    this->spreadIndex = 0;
}

// get value then increment iterator
Result<Value> ArgumentIterator::next(State *state)
{
    if (index >= args.size())
        return Result<Value>(BREAK);
    if (SpreadNode *spreadNode = dynamic_cast<SpreadNode *>(args[index]))
    {
        Wildcard val = spreadNode->getValue(*state).getValue();
        // check for spread object
        if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
        {
            if (Spread<Value> *spread = dynamic_cast<Spread<Value> *>(*x))
            {
                // try to continue in the current spread object
                Result<int> spreadLength = spread->len();
                if (!spreadLength.ok())
                    return Result<Value>("getting spread length:\n" + spreadLength.getError());
                // last index of spread
                int spreadBack = spreadLength.getValue() - 1;
                if (spreadIndex == spreadBack)
                {
                    spreadIndex = 0;
                    ++index;
                    return Result<Value>(spread->at(spreadBack));
                }
                return Result<Value>(spread->at(spreadIndex++));
            }
            else
                return Result<Value>("expected spread object from spread node");
        }
        else
            return Result<Value>("expected iterable from spread node");
    }
    spreadIndex = 0;
    return Result<Value>(args[index++]->getValue(*state));
}

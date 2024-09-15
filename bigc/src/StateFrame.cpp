#include "StateFrame.h"

StateFrame::StateFrame()
{
}

void StateFrame::setVariable(std::string name, Value value)
{
    variables[name] = value;
}

Result<Value> StateFrame::getVariable(std::string name) const
{
    if (variables.find(name) == variables.end())
        return Result<Value>("undefined variable");
    return Result<Value>(variables.at(name));
}

bool StateFrame::isType(std::string word)
{
    return types.find(word) != types.end();
}

Result<bool> StateFrame::implements(std::string type, std::string interface)
{
    if (types.find(type) == types.end())
        return Result<bool>("undefined type");
    return Result<bool>(types[type].find(interface) != types[type].end());
}

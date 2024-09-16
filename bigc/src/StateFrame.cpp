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
    return implementedInterfaces.find(word) != implementedInterfaces.end();
}

Result<bool> StateFrame::implements(std::string type, std::string interface)
{
    // if we don't find the type the state checks the next state frame
    if (implementedInterfaces.find(type) == implementedInterfaces.end())
        return Result<bool>(std::string("undefined type"));
    // if the particular interfaces is in one of the implemented for the type
    return Result<bool>(implementedInterfaces[type].find(interface) != implementedInterfaces[type].end());
}

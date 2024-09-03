#include "State.h"

State::State()
{
}

void State::setVariable(std::string name, Value value)
{
    variables[name] = value;
}

Value State::getVariable(std::string name) const
{
    return variables.at(name);
}

bool State::isKeyword(std::string word)
{
    return false;
}

bool State::isFunction(std::string word)
{
    return false;
}

bool State::implements(std::string type, std::string interface)
{
    return types.find(type) != types.end() && types[type].find(interface) != types[type].end();
}

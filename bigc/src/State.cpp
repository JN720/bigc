#include "State.h"
#include <unordered_set>

const std::unordered_set<std::string> BASE_KEYWORDS({"if", "else", "while"});

State::State()
{
}

void State::setVariable(std::string name, Value value)
{
    variables[name] = value;
}

Result<Value> State::getVariable(std::string name) const
{
    if (variables.find(name) == variables.end())
        return Result<Value>("undefined variable");
    return Result<Value>(variables.at(name));
}

bool State::isKeyword(std::string word)
{
    return BASE_KEYWORDS.find(word) != BASE_KEYWORDS.end();
}

bool State::isFunction(std::string word)
{
    return false;
}

bool State::implements(std::string type, std::string interface)
{
    return types.find(type) != types.end() && types[type].find(interface) != types[type].end();
}

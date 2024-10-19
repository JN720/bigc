#include "Registry.h"

Registry::Registry()
{
    variables = std::unordered_map<std::string, Value>();
}

void Registry::registerVariable(std::string name, Value value)
{
    variables[name] = value;
}

const Result<Value> Registry::getVariable(std::string name) const
{
    if (variables.find(name) == variables.end())
        return Result<Value>("undefined variable");
    return Result<Value>(variables.at(name));
}

size_t Registry::size() const
{
    return variables.size();
}

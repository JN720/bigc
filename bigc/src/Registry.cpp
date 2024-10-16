#include "Registry.h"

Registry::Registry()
{
    groups["global"] = std::unordered_map<std::string, Value>();
}

void Registry::registerVariable(std::string name, Value value)
{
    groups["global"][name] = value;
}

void Registry::registerVariable(std::string name, Value value, std::string group)
{
    if (groups.find(group) == groups.end())
        groups[group] = std::unordered_map<std::string, Value>();
    groups[group][name] = value;
}

const Result<Value> Registry::getVariable(std::string name) const
{
    if (groups.find("global") == groups.end())
        return Result<Value>("undefined variable in global group");

    if (groups.at("global").find(name) == groups.at("global").end())
        return Result<Value>("undefined variable in global group");

    return Result<Value>(groups.at("global").at(name));
}

const Result<Value> Registry::getVariable(std::string name, std::string group) const
{
    if (groups.find(group) == groups.end())
        return Result<Value>("undefined group");

    if (groups.at(group).find(name) == groups.at(group).end())
        return Result<Value>("undefined variable in group");

    return Result<Value>(groups.at(group).at(name));
}
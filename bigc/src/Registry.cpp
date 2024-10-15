#include "Registry.h"

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

const Result<Value> &Registry::getVariable(std::string name) const
{
    if (groups.find("global") == groups.end())
        return Result<Value>("undefined variable in global group " + name);
    return Result<Value>(groups.at("global").at(name));
}

const Result<Value> &Registry::getVariable(std::string name, std::string group) const
{
    if (groups.find(group) == groups.end())
        return Result<Value>("undefined group " + group);
    if (groups.at(group).find(name) == groups.at(group).end())
        return Result<Value>("undefined variable in group " + group + " " + name);
    return Result<Value>(groups.at(group).at(name));
}
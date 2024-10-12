#include "Interface.h"

void Interface::addFunction(std::string name)
{
    methods.insert({name, "fn"});
}

void Interface::addMethod(std::string name, std::string type)
{
    methods.insert({name, type});
}

const std::unordered_map<std::string, std::string> &Interface::getMethods()
{
    return methods;
}

Interface::Interface()
{
}
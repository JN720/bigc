#include "Interface.h"

void Interface::addFunction(std::string name)
{
    methods.insert(name);
}

const std::unordered_set<std::string> &Interface::getFunctions()
{
    return methods;
}

Interface::Interface()
{
}
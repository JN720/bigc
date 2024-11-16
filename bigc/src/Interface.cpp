#include "Interface.h"

void Interface::addFunction(std::string name)
{
    methods.insert({name, "fn"});
}

void Interface::addMethod(std::string name, std::string type)
{
    methods.insert({name, type});
}

void Interface::destroy(State *state)
{
    state->removeRef(this);
}

Interface::Interface()
{
}

const std::unordered_map<std::string, std::string> &Interface::getMethods()
{
    return methods;
}

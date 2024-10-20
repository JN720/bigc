#pragma once
#include <string>

class ClassDefinitionInterface
{
    virtual bool canAccess(std::string property, bool instanced);
};
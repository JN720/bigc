#include "ClassDefinitionInterface.h"
#include <iostream>
#include <exception>

bool ClassDefinitionInterface::canAccess(std::string property, AccessType instanced)
{
    throw "class access not implemented";
    return false;
}

ClassDefinitionInterface *ClassDefinitionInterface::getParent()
{
    return nullptr;
}
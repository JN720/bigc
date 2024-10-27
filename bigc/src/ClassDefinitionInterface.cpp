#include "ClassDefinitionInterface.h"

bool ClassDefinitionInterface::canAccess(std::string property, AccessType instanced)
{
    return false;
}

ClassDefinitionInterface *ClassDefinitionInterface::getParent()
{
    return nullptr;
}
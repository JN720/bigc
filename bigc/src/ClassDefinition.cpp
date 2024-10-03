#include "ClassDefinition.h"

ClassDefinition::ClassDefinition()
{
}

ClassDefinition *ClassDefinition::getParent()
{
    return parentClass;
}

Result<Node *> ClassDefinition::getMethod(std::string name)
{
    if (methods.find(name) == methods.end())
        return Result<Node *>("undefined method");
    return Result<Node *>(methods[name]);
}

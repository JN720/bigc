#include "Object.h"

Object::Object(ClassDefinition *definition)
{
    objClass = definition;
}

ClassDefinition *Object::getClass()
{
    return objClass;
}

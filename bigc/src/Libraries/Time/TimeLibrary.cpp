#include "TimeLibrary.h"
#include "TimeFunctions.h"
#include "../GeneralFunction.h"

Registry *libtime::init()
{
    Registry *registry = new Registry();

    registry->registerVariable("sleep", Value(new lib::GeneralFunction(sleep)));

    return registry;
}
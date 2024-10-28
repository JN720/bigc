#include "ThreadLibrary.h"
#include "ThreadFunctions.h"
#include "ThreadFundamentalClass.h"
#include "../GeneralFunction.h"
#include "../../ClassNode.h"

Registry *libthread::init()
{

    Registry *registry = new Registry();

    registry->registerVariable("Thread", Value(new ClassNode(new ThreadFundamentalClass())));
    registry->registerVariable("async", Value(new lib::GeneralFunction(asyncFunction)));
    registry->registerVariable("await", Value(new lib::GeneralFunction(awaitFunction)));

    return registry;
}
#pragma once
#include "../../Registry.h"
#include <fstream>
#include "../../ClassNode.h"

#if defined(BUILD_CPU) || defined(BUILD_GPU)
namespace libfile
{
    Registry *init();
}
#endif

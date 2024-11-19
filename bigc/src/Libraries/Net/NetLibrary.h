#pragma once
#include "../../Registry.h"

#if defined(BUILD_CPU) || defined(BUILD_GPU)
namespace libnet
{
    Registry *init();
}

#endif

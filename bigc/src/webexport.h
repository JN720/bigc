#pragma once
#include "interpreter.h"

#if defined(WEB)
extern "C"
{
    // the source code should be located in program.lms
    void execute()
    {
        interpreter::interpretFile("program.lms");
    }
}
#endif
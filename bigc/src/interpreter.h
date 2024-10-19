#pragma once

#include "ast.h"

namespace interpreter
{
    void interpretFile(const std::string &filename);
    void debugRepl();
    void repl();
}

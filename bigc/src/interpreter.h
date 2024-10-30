#pragma once

#include "ast.h"

namespace interpreter
{
    void addBasics(State &state);
    void interpretFile(const std::string &filename);
    void debugRepl();
    void repl();
}

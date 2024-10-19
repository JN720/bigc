#include <iostream>
#include <fstream>

#include "interpreter.h"

int main(int argc, char *argv[])
{

    if (argc == 2)
    {
        interpreter::interpretFile(argv[1]);
        return 0;
    }
    interpreter::repl();
    return 0;
}
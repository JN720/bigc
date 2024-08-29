#pragma once
#include "Value.h"

class Iterable
{
    virtual Value get(int index);
};
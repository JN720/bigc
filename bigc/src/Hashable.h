#include "Result.h"

class Hashable
{
public:
    Hashable();
    virtual Result<int> hash();
};
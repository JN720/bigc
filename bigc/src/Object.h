#include <unordered_map>
#include "Value.h"

class Object
{
public:
    Object();
    Object(std::string parentClass);

protected:
    std::string parentClass;
    std::unordered_map<std::string, Value> attributes;
};
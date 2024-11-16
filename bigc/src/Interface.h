#pragma once
#include <unordered_map>
#include <string>
#include "State.h"

class Interface : public Allocated
{
public:
    Interface();
    const std::unordered_map<std::string, std::string> &getMethods();
    void addFunction(std::string name);
    void addMethod(std::string name, std::string type);
    void destroy(State *state);

private:
    std::unordered_map<std::string, std::string> methods;
};
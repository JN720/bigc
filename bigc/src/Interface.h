#pragma once
#include <unordered_map>
#include <string>
#include "Allocated.h"

class Interface : public Allocated
{
public:
    Interface();
    const std::unordered_map<std::string, std::string> &getMethods();
    void addFunction(std::string name);
    void addMethod(std::string name, std::string type);

private:
    std::unordered_map<std::string, std::string> methods;
};
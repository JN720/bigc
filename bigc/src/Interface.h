#pragma once
#include <unordered_set>
#include <string>

class Interface
{
public:
    Interface();
    const std::unordered_set<std::string> &getFunctions();
    void addFunction(std::string name);

private:
    std::unordered_set<std::string> methods;
};
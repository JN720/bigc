#pragma once
#include <unordered_map>
#include <string>

class Interface
{
public:
    Interface();
    const std::unordered_map<std::string, std::string> &getMethods();
    void addFunction(std::string name);
    void addMethod(std::string name, std::string type);

private:
    std::unordered_map<std::string, std::string> methods;
};
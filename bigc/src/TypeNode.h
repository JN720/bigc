#pragma once
#include "Node.h"

class TypeNode : public Node
{
public:
    TypeNode(std::string &type);
    std::string resolve(State &state) override;
    std::string getName();
    std::string getArgType();

private:
    std::string argType;
};
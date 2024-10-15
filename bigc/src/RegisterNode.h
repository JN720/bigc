#pragma once
#include "Node.h"

// this node is used to register variables in the registry
// it has a name and a value as children
class RegisterNode : public Node
{
public:
    RegisterNode(std::string name);
    Control resolve(State &state) override;

private:
    std::string name;
};

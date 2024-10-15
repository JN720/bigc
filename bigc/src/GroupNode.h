#pragma once
#include "Node.h"

// this functions like a namespace
class GroupNode : public Node
{
public:
    GroupNode(std::string name);
    Control resolve(State &state) override;

private:
    std::string name;
};
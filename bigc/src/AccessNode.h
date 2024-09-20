#pragma once
#include "Node.h"
#include "Token.h"

class AccessNode : public Node
{
public:
    AccessNode(std::string property);
    Control resolve(State &state) override;

protected:
    std::string property;
};

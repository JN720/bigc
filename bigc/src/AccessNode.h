#pragma once
#include "Node.h"
#include "Token.h"

class AccessNode : public Node
{
public:
    AccessNode(std::string property);
    Value getValue(const State &state) override;

protected:
    std::string property;
};

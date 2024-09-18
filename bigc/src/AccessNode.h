#pragma once
#include "Node.h"
#include "Token.h"

class AccessNode : public Node
{
public:
    AccessNode(std::string property);

protected:
    std::string property;
};

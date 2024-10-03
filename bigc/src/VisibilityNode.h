#pragma once
#include "VariableNode.h"

enum AccessSpecifier
{
    PUBLIC,
    PRIVATE,
    PROTECTED
};

// this specifies access as public, private, or protected
class VisibilityNode : public VariableNode
{
public:
    VisibilityNode();
    VisibilityNode(AccessSpecifier specifier);
    VisibilityNode(std::string specifier);

    AccessSpecifier getVisibility();
    Control resolve(State &state);
    std::string getVariable() override;
    void makeStatic();

protected:
    AccessSpecifier access;
    bool isStatic;
};
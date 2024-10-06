#pragma once
#include "VariableNode.h"
#include "ClassDefinition.h"

// this specifies access as public, private, or protected
class VisibilityNode : public VariableNode
{
public:
    VisibilityNode();
    VisibilityNode(AccessSpecifier specifier);
    VisibilityNode(std::string specifier);

    AccessSpecifier getVisibility();
    Control resolve(State &state) override;
    std::string getVariable() override;
    void makeStatic();
    bool getIsStatic();
    void applyToDefinition(ClassDefinition *definition);
    void makeMethod();

protected:
    AccessSpecifier access;
    bool isStatic;
    bool isMethod;
};
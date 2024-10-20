#pragma once
#include "VariableNode.h"
#include "ClassDefinition.h"

// this specifies access as public, private, or protected
// it also specifies whether the variable is static
// it may contain a type
// it may have 1 child which is a default value
class VisibilityNode : public VariableNode
{
public:
    VisibilityNode();
    VisibilityNode(AccessSpecifier specifier);
    VisibilityNode(std::string specifier);

    AccessSpecifier getVisibility();
    Control resolve(State &state) override;
    void makeStatic();
    bool getIsStatic();
    Control applyToDefinition(ClassDefinition *definition, const State &state);
    void makeMethod();
    void setType(std::string type);
    std::string getAttributeType();
    bool getIsMethod();

protected:
    AccessSpecifier access;
    bool isStatic;
    bool isMethod;
    std::string attributeType;
};

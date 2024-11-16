#pragma once
#include "Node.h"
#include "ClassDefinition.h"

// this produces the class definition
class ClassNode : public Node
{
public:
    ClassNode();
    ClassNode(ClassDefinition *definition);
    ClassNode(ClassDefinitionInterface *definition);
    ClassDefinition *getClassDefinition();
    Control resolve(State &state) override;
    void destroy(State *state) override;

protected:
    ClassDefinition *definition;
};
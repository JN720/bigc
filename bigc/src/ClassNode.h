#pragma once
#include "Node.h"

// this produces the class definition
class ClassNode : public Node
{
public:
    ClassNode();
    ClassDefinition *getClassDefinition();
    Control resolve(State &state) override;

protected:
    ClassDefinition *definition;
};
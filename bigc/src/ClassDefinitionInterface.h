#pragma once
#include <string>
#include "Allocated.h"

class State;

enum AccessType
{
    OUTSIDE,
    THIS,
    SUPER
};

const std::string ACCESS_TYPES[] = {
    "outside",
    "this",
    "super"};

class ClassDefinitionInterface : Allocated
{
public:
    virtual bool canAccess(std::string property, AccessType instanced);
    virtual ClassDefinitionInterface *getParent();
    virtual void destroy(State *state) override;
};
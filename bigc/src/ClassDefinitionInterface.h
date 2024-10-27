#pragma once
#include <string>

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

class ClassDefinitionInterface
{
public:
    virtual bool canAccess(std::string property, AccessType instanced);
    virtual ClassDefinitionInterface *getParent();
};
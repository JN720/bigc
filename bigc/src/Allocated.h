#pragma once
#include <cstddef>
#include <functional>
#include <cstdint>

class State;

class Allocated
{
public:
    virtual void destroy(State *state);
};

struct AllocatedHash
{
    std::size_t operator()(const Allocated *allocated) const
    {
        return std::hash<std::uintptr_t>()(reinterpret_cast<std::uintptr_t>(allocated));
    }
};

struct AllocatedEqual
{
    bool operator()(const Allocated *lhs, const Allocated *rhs) const
    {
        return lhs == rhs;
    }
};
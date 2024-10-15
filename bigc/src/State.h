#pragma once
#include <forward_list>
#include "StateFrame.h"
#include "Registry.h"
class State
{
public:
    State();
    void setVariable(std::string name, Value value);
    Result<Value> getVariable(std::string name) const;
    bool isKeyword(std::string name);
    bool isType(std::string name);
    // bool implements(std::string type, std::string interface);
    StateFrame *pushFrame(bool closure);
    void popFrame();
    void listVars();
    void registerVariable(std::string name, Value value);
    bool isBuiltIn(std::string name);
    void registerVariable(std::string name, Value value, std::string group);
    void setGroup(std::string group);

private:
    std::forward_list<StateFrame *> states;
    // immutable registry of global variables and functions
    Registry *registry;
    std::string currentGroup;
};

#pragma once
#include <forward_list>
#include "StateFrame.h"
class State
{
public:
    State();
    void setVariable(std::string name, Value value);
    Result<Value> getVariable(std::string name) const;
    bool isKeyword(std::string word);
    bool isType(std::string word);
    // bool implements(std::string type, std::string interface);
    StateFrame *pushFrame();
    void popFrame();
    void listVars();

private:
    std::forward_list<StateFrame *> states;
};

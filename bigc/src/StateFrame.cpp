#include "StateFrame.h"

StateFrame::StateFrame(bool closure)
{
    this->closure = closure;
}

void StateFrame::setVariable(std::string name, Value value)
{
    this->variables[name] = value;
}

Result<Value> StateFrame::getVariable(std::string name) const
{
    if (variables.find(name) == variables.end())
        return Result<Value>("undefined variable");
    return Result<Value>(this->variables.at(name));
}

void StateFrame::listVars()
{
    for (auto pair : variables)
    {
        std::cout << pair.first << ", " << pair.second.getType();
        auto thing = pair.second.getValue();
        if (int *x = std::get_if<int>(&thing))
            std::cout << ", " << *x;
        std::cout << '\n';
    }
}

bool StateFrame::isClosure() const
{
    return closure;
}

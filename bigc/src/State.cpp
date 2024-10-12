#include "State.h"
#include "Interface.h"

const std::unordered_set<std::string> BASE_KEYWORDS({"if", "else", "while", "collect", "funion", "break", "continue", "return", "class", "public", "private", "protected", "utility", "shared", "method"});
const std::unordered_set<std::string> FUNDAMENTAL_TYPES({"int", "char", "long", "str", "float", "double", "arr", "fn"});
const std::string FUNDAMENTAL_FUNCTIONS[] = {"print", "println", "len", "type", "input"};

State::State()
{
    states.push_front(new StateFrame());

    registry = new StateFrame();
    registry->setVariable("true", Value(true));
    registry->setVariable("false", Value(false));
}

void State::registerVariable(std::string name, Value value)
{
    registry->setVariable(name, value);
}

bool State::isBuiltIn(std::string name)
{
    for (int i = 0; i < sizeof(FUNDAMENTAL_FUNCTIONS) / sizeof(FUNDAMENTAL_FUNCTIONS[0]); i++)
    {
        if (FUNDAMENTAL_FUNCTIONS[i] == name)
            return true;
    }
    return false;
}

void State::setVariable(std::string name, Value value)
{
    // search for existing variable
    for (StateFrame *state : states)
    {
        if (state->getVariable(name).ok())
        {
            state->setVariable(name, value);
            return;
        }
    }
    // if none exists, add it to the current scope
    states.front()->setVariable(name, value);
}

Result<Value> State::getVariable(std::string name) const
{
    for (StateFrame *frame : states)
    {
        Result<Value> result = frame->getVariable(name);
        if (result.ok())
            return Result<Value>(result.getValue());
    }
    return Result<Value>("undefined variable");
}

bool State::isKeyword(std::string word)
{
    return BASE_KEYWORDS.find(word) != BASE_KEYWORDS.end();
}

bool State::isType(std::string word)
{
    if (FUNDAMENTAL_TYPES.find(word) != FUNDAMENTAL_TYPES.end())
        return true;
    return false;
}
/*
bool State::implements(std::string type, std::string interface)
{
    for (StateFrame *state : states)
    {
        Result<bool> result = state->implements(type, interface);
        if (result.ok())
            return result.getValue();
    }
    return false;
}
*/
StateFrame *State::pushFrame()
{
    states.push_front(new StateFrame());
    return states.front();
}

void State::popFrame()
{
    delete states.front();
    states.pop_front();
}

void State::listVars()
{
    int i = 0;
    for (StateFrame *frame : states)
    {
        std::cout << "state frame " << i << '\n';
        frame->listVars();
        ++i;
    }
}

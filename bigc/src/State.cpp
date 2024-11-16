#include "State.h"
#include "Interface.h"

const std::unordered_set<std::string> BASE_KEYWORDS({"if", "else", "while", "collect", "funion", "break", "continue",
                                                     "return", "class", "public", "private", "protected", "utility",
                                                     "shared", "method", "interface", "group", "register", "iterate",
                                                     "try"});
const std::unordered_set<std::string> FUNDAMENTAL_TYPES({"int", "char", "long", "str", "float", "double", "arr", "fn"});
const std::string FUNDAMENTAL_FUNCTIONS[] = {"print", "println", "len", "type", "input", "import", "include"};

State::State()
{
    currentGroup = "global";
    states.push_front(new StateFrame(true));

    registry = new Registry();
}

void State::registerVariable(std::string name, Value value)
{
    registry->registerVariable(name, value);
}

void State::setGroup(std::string group)
{
    currentGroup = group;
}

Registry *State::getRegistry()
{
    return registry;
}

void State::addRef(Allocated *allocated)
{
    // std::cout << "adding ref to " << allocated << '\n';
    refs.insert(allocated);
    // std::cout << "count: " << refs.count(allocated) << '\n';
}

void State::removeRef(Allocated *allocated)
{
    // std::cout << "removing ref to " << allocated << '\n';
    refs.extract(allocated);
    // std::cout << "count: " << refs.count(allocated) << '\n';
    // free the memory
    if (!refs.count(allocated))
    {
        // std::cout << "deleting " << allocated << '\n';
        delete allocated;
        // std::cout << "delete finished\n";
    }
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
        Result<Value> result = state->getVariable(name);
        if (result.ok())
        {
            // delete old ref
            if (Allocated *x = getAllocated(result.getValue()))
                removeRef(x);
            state->setVariable(name, value);
            // add new ref
            if (Allocated *x = getAllocated(value))
                addRef(x);
            return;
        }
        if (state->isClosure())
            break;
    }
    // check the registry
    // registered variables are not allowed to be reassigned
    Result<Value> registered = registry->getVariable(name);
    if (registered.ok())
        return;
    // if none exists, add it to the current scope
    states.front()->setVariable(name, value);
    if (Allocated *x = getAllocated(value))
        addRef(x);
}

Result<Value> State::getVariable(std::string name) const
{
    for (StateFrame *frame : states)
    {
        Result<Value> result = frame->getVariable(name);
        if (result.ok())
            return Result<Value>(result.getValue());
        if (frame->isClosure())
            break;
    }
    // check registry
    Result<Value> registered = registry->getVariable(name);
    if (registered.ok())
        return registered;
    return Result<Value>("undefined variable");
}

bool State::isKeyword(std::string word)
{
    return BASE_KEYWORDS.find(word) != BASE_KEYWORDS.end();
}

bool State::isType(std::string name)
{
    if (FUNDAMENTAL_TYPES.find(name) != FUNDAMENTAL_TYPES.end())
        return true;
    // check registry
    // for a class or interface to be recognized as a type, it must be registered
    Result<Value> registered = registry->getVariable(name);
    if (registered.ok())
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
StateFrame *State::pushFrame(bool closure)
{
    states.push_front(new StateFrame(closure));
    return states.front();
}

void State::popFrame()
{
    for (auto &pair : states.front()->getVariables())
    {
        if (Allocated *x = getAllocated(pair.second))
            removeRef(x);
    }
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

// returns nullptr if not an allocated type
Allocated *State::getAllocated(Value value)
{
    Wildcard val = value.getValue();
    if (Node **x = std::get_if<Node *>(&val))
        return (Allocated *)(*x);
    if (Object **x = std::get_if<Object *>(&val))
        return (Allocated *)(*x);
    if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
        return (Allocated *)(*x);
    return nullptr;
}
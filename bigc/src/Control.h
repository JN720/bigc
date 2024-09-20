#pragma once
#include <string>

enum Signal
{
    OK,
    EXIT,
    RETURN,
    BREAK,
    CONTINUE,
    ERROR
};

class Control
{
public:
    Control();
    Control(std::string err);
    Control(Signal signal);
    Control stack(std::string err);
    Signal getSignal();
    std::string getError();
    bool ok();
    bool error();
    bool control();

private:
    std::string e;
    Signal signal;
};
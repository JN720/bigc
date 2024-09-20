#include "Control.h"

Control::Control()
{
    signal = OK;
}

Control::Control(Signal signal)
{
    this->signal = signal;
}

Control::Control(std::string err)
{
    e = err;
    signal = ERROR;
}

Control Control::stack(std::string err)
{
    e = err + '\n' + e;
    return *this;
}

Signal Control::getSignal()
{
    return signal;
}

bool Control::ok()
{
    return signal == OK;
}

std::string Control::getError()
{
    return e;
}

bool Control::error()
{
    return signal == ERROR;
}

bool Control::control()
{
    return signal != ERROR && signal != OK;
}
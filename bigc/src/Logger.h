#pragma once

#include <iostream>
#include <fstream>
#include <string>

enum class LogMode
{
    STANDARD,
    FILE
};

class Logger
{
public:
    Logger();
    Logger(const std::string &filename);
    void log(const std::string &message);
    void flush();

private:
    LogMode mode;
    std::string filename;
};

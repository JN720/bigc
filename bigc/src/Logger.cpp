#include "Logger.h"

Logger::Logger(const std::string &filename)
{
    this->mode = LogMode::FILE;
    this->filename = filename;
}

Logger::Logger()
{
    mode = LogMode::FILE;
    filename = "";
}

void Logger::log(const std::string &message)
{
    if (mode == LogMode::STANDARD)
        std::cout << message;
    else if (mode == LogMode::FILE)
    {
        std::ofstream outFile(filename, std::ios::app);
        outFile << message;
    }
}

void Logger::flush()
{
    if (mode == LogMode::STANDARD)
        std::cout.flush();
    else if (mode == LogMode::FILE)
    {
        std::ofstream outFile(filename);
        outFile.flush();
    }
}
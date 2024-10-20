#include "FileFundamentalObject.h"

FileFundamentalObject::FileFundamentalObject(const std::string &filename, const std::string &mode)
{
    std::ios_base::openmode openMode = std::ios_base::in;
    if (mode == "w")
        openMode = std::ios_base::out;
    else if (mode == "a")
        openMode = std::ios_base::app;

    fileStream.open(filename, openMode);
}

FileFundamentalObject::~FileFundamentalObject()
{
    if (fileStream.is_open())
    {
        fileStream.close();
    }
}

void FileFundamentalObject::close()
{
    if (fileStream.is_open())
    {
        fileStream.close();
    }
}

Result<Value> FileFundamentalObject::getProperty(const std::string &property)
{
    if (property == "isOpen")
    {
        return Result<Value>(Value(isOpen()));
    }
    return Result<Value>("Property not found: " + property);
}

Control FileFundamentalObject::setProperty(const std::string &property, const Value &value)
{
    return Control(std::string("Cannot set properties on FileFundamentalObject"));
}

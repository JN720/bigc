#pragma once
#include "../../FundamentalObject.h"
#include <fstream>

class FileFundamentalObject : public FundamentalObject
{
public:
    FileFundamentalObject(const std::string &filename, const std::string &mode);
    ~FileFundamentalObject();

    std::fstream &getStream() { return fileStream; }
    bool isOpen() const { return fileStream.is_open(); }
    void close();

    Result<Value> getProperty(const std::string &property) override;
    Control setProperty(const std::string &property, const Value &value) override;

private:
    std::fstream fileStream;
};
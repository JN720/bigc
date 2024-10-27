#pragma once

#include "../../Registry.h"
#include "../../Value.h"
#include "../../Result.h"
#include <string>
#include <vector>
#include <filesystem>

namespace libos
{
    Registry *init();

    // Function declarations
    Result<Value> getCurrentWorkingDirectory();
    Result<Value> listFilesInDirectory(const std::string &path);
    Result<Value> fileExists(const std::string &path);
    Result<Value> createDirectory(const std::string &path);
    Result<Value> deleteFile(const std::string &path);
    Result<Value> renameFile(const std::string &oldName, const std::string &newName);
    Result<Value> getEnvironmentVariable(const std::string &varName);
}

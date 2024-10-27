#include "OSLibrary.h"
#include "../GeneralFunction.h"
#include "../../Array.h"
#include <filesystem>
#include <cstdlib>

namespace libos
{
    Registry *init()
    {
        Registry *registry = new Registry();

        // Register OS functions
        registry->registerVariable("getCurrentWorkingDirectory", Value(new lib::GeneralFunction([](State &state, std::vector<Node *> &args) -> Result<Value>
                                                                                                { return getCurrentWorkingDirectory(); })));

        registry->registerVariable("listFilesInDirectory", Value(new lib::GeneralFunction([](State &state, std::vector<Node *> &args) -> Result<Value>
                                                                                          {
            if (args.size() != 1)
                return Result<Value>("listFilesInDirectory requires one argument: path");

            Wildcard arg = args[0]->getValue(state).getValue();
            if (std::string **path = std::get_if<std::string *>(&arg))
                return listFilesInDirectory(**path);

            return Result<Value>("expected string argument for path"); })));

        registry->registerVariable("fileExists", Value(new lib::GeneralFunction([](State &state, std::vector<Node *> &args) -> Result<Value>
                                                                                {
            if (args.size() != 1)
                return Result<Value>("fileExists requires one argument: path");

            Wildcard arg = args[0]->getValue(state).getValue();
            if (std::string **path = std::get_if<std::string *>(&arg))
                return fileExists(**path);

            return Result<Value>("expected string argument for path"); })));

        registry->registerVariable("createDirectory", Value(new lib::GeneralFunction([](State &state, std::vector<Node *> &args) -> Result<Value>
                                                                                     {
            if (args.size() != 1)
                return Result<Value>("createDirectory requires one argument: path");

            Wildcard arg = args[0]->getValue(state).getValue();
            if (std::string **path = std::get_if<std::string *>(&arg))
                return createDirectory(**path);

            return Result<Value>("expected string argument for path"); })));

        registry->registerVariable("deleteFile", Value(new lib::GeneralFunction([](State &state, std::vector<Node *> &args) -> Result<Value>
                                                                                {
            if (args.size() != 1)
                return Result<Value>("deleteFile requires one argument: path");

            Wildcard arg = args[0]->getValue(state).getValue();
            if (std::string **path = std::get_if<std::string *>(&arg))
                return deleteFile(**path);

            return Result<Value>("expected string argument for path"); })));

        registry->registerVariable("renameFile", Value(new lib::GeneralFunction([](State &state, std::vector<Node *> &args) -> Result<Value>
                                                                                {
            if (args.size() != 2)
                return Result<Value>("renameFile requires two arguments: oldName and newName");

            Wildcard oldArg = args[0]->getValue(state).getValue();
            Wildcard newArg = args[1]->getValue(state).getValue();
            if (std::string **oldName = std::get_if<std::string *>(&oldArg))
            {
                if (std::string **newName = std::get_if<std::string *>(&newArg))
                    return renameFile(**oldName, **newName);
            }

            return Result<Value>("expected string arguments for oldName and newName"); })));

        registry->registerVariable("getEnvironmentVariable", Value(new lib::GeneralFunction([](State &state, std::vector<Node *> &args) -> Result<Value>
                                                                                            {
            if (args.size() != 1)
                return Result<Value>("getEnvironmentVariable requires one argument: variable name");

            Wildcard arg = args[0]->getValue(state).getValue();
            if (std::string **varName = std::get_if<std::string *>(&arg))
                return getEnvironmentVariable(**varName);

            return Result<Value>("expected string argument for variable name"); })));

        return registry;
    }

    Result<Value> getCurrentWorkingDirectory()
    {
        try
        {
            std::string cwd = std::filesystem::current_path().string();
            return Result<Value>(Value(new std::string(cwd)));
        }
        catch (const std::exception &e)
        {
            return Result<Value>(std::string("Error getting current working directory: ") + e.what());
        }
    }

    Result<Value> listFilesInDirectory(const std::string &path)
    {
        try
        {
            Array<Value> *files = new Array<Value>();
            for (const auto &entry : std::filesystem::directory_iterator(path))
            {
                Control control = files->add(Value(new std::string(entry.path().filename().string())));
                if (!control.ok())
                    return Result<Value>(control.stack("Error listing files in directory: "));
            }
            return Result<Value>(Value(files));
        }
        catch (const std::exception &e)
        {
            return Result<Value>("Error listing files in directory: " + std::string(e.what()));
        }
    }

    Result<Value> fileExists(const std::string &path)
    {
        return Result<Value>(Value(std::filesystem::exists(path)));
    }

    Result<Value> createDirectory(const std::string &path)
    {
        try
        {
            if (std::filesystem::create_directory(path))
                return Result<Value>(Value(true));
            else
                return Result<Value>("Directory already exists or could not be created.");
        }
        catch (const std::exception &e)
        {
            return Result<Value>("Error creating directory: " + std::string(e.what()));
        }
    }

    Result<Value> deleteFile(const std::string &path)
    {
        try
        {
            if (std::filesystem::remove(path))
                return Result<Value>(Value(true));
            else
                return Result<Value>("File does not exist or could not be deleted.");
        }
        catch (const std::exception &e)
        {
            return Result<Value>("Error deleting file: " + std::string(e.what()));
        }
    }

    Result<Value> renameFile(const std::string &oldName, const std::string &newName)
    {
        try
        {
            std::filesystem::rename(oldName, newName);
            return Result<Value>(Value(true));
        }
        catch (const std::exception &e)
        {
            return Result<Value>("Error renaming file: " + std::string(e.what()));
        }
    }

    Result<Value> getEnvironmentVariable(const std::string &varName)
    {
        const char *value = std::getenv(varName.c_str());
        if (value)
            return Result<Value>(Value(new std::string(value)));
        else
            return Result<Value>("Environment variable not found.");
    }
}

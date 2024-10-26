#include "FileOperations.h"
#include <filesystem>

Result<Value> ReadFileFunction::execute(State &state, std::vector<Node *> &args)
{
    if (args.size() != 1)
        return Result<Value>("readFile requires one argument: filename");

    Control control = resolveArguments(state, args);
    if (control.control())
        return Result<Value>(control);
    if (control.error())
        return Result<Value>("resolving arguments:\n" + control.error());

    Wildcard val = args[0]->getValue(state).getValue();
    std::string **filename = std::get_if<std::string *>(&val);
    if (!filename || !*filename)
        return Result<Value>("readFile requires a string argument: filename");

    std::ifstream file(**filename);
    if (!file.is_open())
        return Result<Value>("Failed to open file: " + **filename);

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return Result<Value>(Value(new std::string(content)));
}

Result<Value> WriteFileFunction::execute(State &state, std::vector<Node *> &args)
{
    if (args.size() != 2)
        return Result<Value>("writeFile requires two arguments: filename and content");

    Control control = resolveArguments(state, args);
    if (control.control())
        return Result<Value>(control);
    if (control.error())
        return Result<Value>("resolving arguments:\n" + control.error());

    Wildcard filenameVal = args[0]->getValue(state).getValue();
    std::string **filename = std::get_if<std::string *>(&filenameVal);
    if (!filename || !*filename)
        return Result<Value>("writeFile requires a string argument: filename");

    Wildcard contentVal = args[1]->getValue(state).getValue();
    std::string **content = std::get_if<std::string *>(&contentVal);
    if (!content || !*content)
        return Result<Value>("writeFile requires a string argument: content");

    std::ofstream file(**filename);
    if (!file.is_open())
        return Result<Value>("Failed to open file: " + **filename);

    file << **content;
    return Result<Value>(Value(true));
}

Result<Value> AppendFileFunction::execute(State &state, std::vector<Node *> &args)
{
    if (args.size() != 2)
        return Result<Value>("appendFile requires two arguments: filename and content");

    Control control = resolveArguments(state, args);
    if (control.control())
        return Result<Value>(control);
    if (control.error())
        return Result<Value>("resolving arguments:\n" + control.error());

    Wildcard filenameVal = args[0]->getValue(state).getValue();
    std::string **filename = std::get_if<std::string *>(&filenameVal);
    if (!filename || !*filename)
        return Result<Value>("appendFile requires a string argument: filename");
    Wildcard contentVal = args[1]->getValue(state).getValue();
    std::string **content = std::get_if<std::string *>(&contentVal);
    if (!content || !*content)
        return Result<Value>("appendFile requires a string argument: content");

    std::ofstream file(**filename, std::ios_base::app);
    if (!file.is_open())
        return Result<Value>("Failed to open file: " + **filename);

    file << **content;
    return Result<Value>(Value(true));
}

Result<Value> FileExistsFunction::execute(State &state, std::vector<Node *> &args)
{
    if (args.size() != 1)
        return Result<Value>("fileExists requires one argument: filename");

    Control control = resolveArguments(state, args);
    if (control.control())
        return Result<Value>(control);
    if (control.error())
        return Result<Value>("resolving arguments:\n" + control.error());

    Wildcard val = args[0]->getValue(state).getValue();
    std::string **filename = std::get_if<std::string *>(&val);
    if (!filename || !*filename)
        return Result<Value>("fileExists requires a string argument: filename");

    return Result<Value>(Value(std::filesystem::exists(**filename)));
}

Result<Value> DeleteFileFunction::execute(State &state, std::vector<Node *> &args)
{
    if (args.size() != 1)
        return Result<Value>("deleteFile requires one argument: filename");

    Control control = resolveArguments(state, args);
    if (control.control())
        return Result<Value>(control);
    if (control.error())
        return Result<Value>("resolving arguments:\n" + control.error());

    Wildcard val = args[0]->getValue(state).getValue();
    std::string **filename = std::get_if<std::string *>(&val);
    if (!filename || !*filename)
        return Result<Value>("deleteFile requires a string argument: filename");

    return Result<Value>(Value(std::filesystem::remove(**filename)));
}

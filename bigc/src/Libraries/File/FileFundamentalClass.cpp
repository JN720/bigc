#include "FileFundamentalClass.h"
#include "FileFundamentalMethodNode.h"
#include <iterator>

#if defined(BUILD_CPU) || defined(BUILD_GPU)
FileFundamentalClass::FileFundamentalClass()
{
    addMethod("read", new FileFundamentalMethodNode([](FileFundamentalObject *file, State &state, std::vector<Node *> &args) -> Result<Value>
                                                    {
        if (!file->isOpen()) return Result<Value>("File is not open");
        std::string content((std::istreambuf_iterator<char>(file->getStream())), std::istreambuf_iterator<char>());
        return Result<Value>(Value(new std::string(content))); }),
              false, PUBLIC);

    // Commented out methods can be added here when implemented
    // addMethod("write", ...);
    // addMethod("close", ...);
}

Result<Value> FileFundamentalClass::construct(State *state, std::vector<Node *> &args)
{
    if (args.size() != 2)
        return Result<Value>("File constructor requires two arguments: filename and mode");

    Result<Value> filenameResult = args[0]->getValue(*state);
    if (!filenameResult.ok())
        return filenameResult;
    Wildcard filenameValue = filenameResult.getValue().getValue();
    std::string **filename = std::get_if<std::string *>(&filenameValue);
    if (!filename || !*filename || **filename == "")
        return Result<Value>("Invalid filename type");

    Result<Value> modeResult = args[1]->getValue(*state);
    if (!modeResult.ok())
        return modeResult;
    Wildcard modeValue = modeResult.getValue().getValue();
    std::string **mode = std::get_if<std::string *>(&modeValue);
    if (!mode || !*mode)
        return Result<Value>("Invalid mode type");

    FileFundamentalObject *fileObj = new FileFundamentalObject(**filename, **mode);
    if (!fileObj->isOpen())
    {
        delete fileObj;
        return Result<Value>("Failed to open file: " + **filename);
    }

    return Result<Value>(Value((Object *)fileObj));
}
#endif

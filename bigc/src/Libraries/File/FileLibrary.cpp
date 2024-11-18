#include "FileLibrary.h"
#include "FileFundamentalClass.h"
#include "FileOperations.h"

#if defined(BUILD_CPU) || defined(BUILD_GPU)
Registry *libfile::init()
{
    Registry *registry = new Registry();

    // Register the File fundamental class
    registry->registerVariable("File", Value(new ClassNode(new FileFundamentalClass())));

    // Register file operations
    registry->registerVariable("readFile", Value(new ReadFileFunction()));
    registry->registerVariable("writeFile", Value(new WriteFileFunction()));
    registry->registerVariable("appendFile", Value(new AppendFileFunction()));
    registry->registerVariable("fileExists", Value(new FileExistsFunction()));
    registry->registerVariable("deleteFile", Value(new DeleteFileFunction()));

    return registry;
}
#endif

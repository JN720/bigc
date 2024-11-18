#include "FileFundamentalMethodNode.h"

#if defined(BUILD_CPU) || defined(BUILD_GPU)
FileFundamentalMethodNode::FileFundamentalMethodNode()
{
    method = [](FileFundamentalObject *file, State &state, std::vector<Node *> &args) -> Result<Value>
    {
        return Result<Value>("FileFundamentalMethodNode cannot be executed without a function");
    };
}

FileFundamentalMethodNode::FileFundamentalMethodNode(FileMethodFunction func)
{
    method = func;
}
#endif

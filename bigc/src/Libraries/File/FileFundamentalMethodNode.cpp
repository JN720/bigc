#include "FileFundamentalMethodNode.h"

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

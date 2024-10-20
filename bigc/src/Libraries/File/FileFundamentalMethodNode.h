#pragma once

#include <functional>
#include "../../FundamentalMethodNode.h"
#include "FileFundamentalClass.h"

class FileFundamentalMethodNode : public FundamentalMethodNode
{
public:
    using FileMethodFunction = std::function<Result<Value>(FileFundamentalObject *, State &, std::vector<Node *> &)>;
    FileFundamentalMethodNode();
    FileFundamentalMethodNode(FileMethodFunction func);

private:
    FileMethodFunction method;
};

#pragma once
#include "../../FunctionNode.h"
#include <fstream>

#if defined(BUILD_CPU) || defined(BUILD_GPU)
class ReadFileFunction : public FunctionNode
{
public:
    Result<Value> execute(State &state, std::vector<Node *> &args) override;
};

class WriteFileFunction : public FunctionNode
{
public:
    Result<Value> execute(State &state, std::vector<Node *> &args) override;
};

class AppendFileFunction : public FunctionNode
{
public:
    Result<Value> execute(State &state, std::vector<Node *> &args) override;
};

class FileExistsFunction : public FunctionNode
{
public:
    Result<Value> execute(State &state, std::vector<Node *> &args) override;
};

class DeleteFileFunction : public FunctionNode
{
public:
    Result<Value> execute(State &state, std::vector<Node *> &args) override;
};
#endif

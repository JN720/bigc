#pragma once
#include "Node.h"

enum Operation
{
    ASS,
    CAST,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    EQ,
    NEQ,
    LT,
    LTE,
    GT,
    GTE,
    NOT
};

class OperationNode : public Node
{
public:
    std::string resolve(State &state) override;
    OperationNode(Token &token);

protected:
    Operation op;
};

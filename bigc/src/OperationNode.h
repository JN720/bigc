#pragma once
#include "Node.h"

enum Operation
{
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
    Value getValue(const State &state) override;
    OperationNode(Token &token);

protected:
    Operation op;
};

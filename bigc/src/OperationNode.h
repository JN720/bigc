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
    NOT,
    ADDASS,
    SUBASS,
    MULASS,
    DIVASS,
    MODASS
};

class OperationNode : public Node
{
public:
    Control resolve(State &state) override;
    OperationNode(Token &token);

protected:
    Operation op;
};

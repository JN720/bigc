#include "OperationNode.h"

Value OperationNode::getValue(const State &state)
{
    switch (op)
    {
    case ADD:

    case SUB:

    case MUL:

    default:
        return Value();
    }
}

OperationNode::OperationNode(Token &token)
{
    if (token.type != OPERATOR)
        throw "rip";
    if (token.value == "+")
        op = ADD;
    else if (token.value == "-")
        op = SUB;
    else if (token.value == "*")
        op = MUL;
    else if (token.value == "/")
        op = DIV;
    else if (token.value == "%")
        op = MOD;
    else if (token.value == "==")
        op = EQ;
    else if (token.value == "!=")
        op = NEQ;
    else if (token.value == "<")
        op = LT;
    else if (token.value == "<=")
        op = LTE;
    else if (token.value == ">")
        op = GT;
    else if (token.value == ">=")
        op = GTE;
    else if (token.value == "!")
        op = NOT;
}

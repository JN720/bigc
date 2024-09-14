#include "OperationNode.h"
#include "IdentifierNode.h"

std::string OperationNode::resolve(State &state)
{
    if (children.empty())
        return "no operands";
    // do not do this for assignment
    if (op != ASS)
    {
        for (auto child : children)
        {
            std::string error = child->resolve(state);
            if (!error.empty())
                return "during operation:\n" + error;
        }
    }
    Result<Value> result("failed to perform operation");
    std::string error;
    switch (op)
    {
    case ASS:
        if (children.size() != 2)
            return "invalid number of operands for assignment: '" + std::to_string(children.size()) + '\'';
        // resolve right hand side of assignment operator
        error = children[1]->resolve(state);
        if (!error.empty())
            return "during assignment:\n" + error;
        if (dynamic_cast<IdentifierNode *>(children[0]))
        {
            state.setVariable(((IdentifierNode *)children[0])->getVariable(), children[1]->getValue(state));
        }
        value = children[1]->getValue(state);
        return "";
    case ADD:
        if (children.size() == 2)
            result = children[0]->getValue(state).add(children[1]->getValue(state));
        else
            result = children[0]->getValue(state);
        break;
    case SUB:
        if (children.size() == 2)
            result = children[0]->getValue(state).subtract(children[1]->getValue(state));
        else
            result = children[0]->getValue(state).negate();
        break;
    case MUL:
        if (children.size() == 2)
            result = children[0]->getValue(state).multiply(children[1]->getValue(state));
        else
            result = children[0]->getValue(state);
        break;
    case DIV:
        if (children.size() == 2)
            result = children[0]->getValue(state).divide(children[1]->getValue(state));
        else
            result = children[0]->getValue(state).reciprocate();
        break;
    case MOD:
        if (children.size() == 2)
            result = children[0]->getValue(state).modulo(children[1]->getValue(state));
        break;
    case EQ:
        if (children.size() == 2)
            result = children[0]->getValue(state).isEqual(children[1]->getValue(state));
        break;
    case NEQ:
        if (children.size() == 2)
            result = children[0]->getValue(state).isNotEqual(children[1]->getValue(state));
        break;
    case LT:
        if (children.size() == 2)
            result = children[0]->getValue(state).isLessThan(children[1]->getValue(state));
        break;
    case LTE:
        if (children.size() == 2)
            result = children[0]->getValue(state).isLessThanEqual(children[1]->getValue(state));
        break;
    case GT:
        if (children.size() == 2)
            result = children[0]->getValue(state).isGreaterThan(children[1]->getValue(state));
        break;
    case GTE:
        if (children.size() == 2)
            result = children[0]->getValue(state).isGreaterThanEqual(children[1]->getValue(state));
        break;
    case NOT:
        if (children.size() == 1)
            result = children[0]->getValue(state).negate();
        break;
    default:
        return "no operator specified";
    }
    if (!result.ok())
        return "during operation:\n" + result.getError();
    value = result.getValue();
    return "";
}

OperationNode::OperationNode(Token &token)
{
    if (token.type != OPERATOR)
        throw "rip";
    if (token.value == "=")
        op = ASS;
    else if (token.value == "+")
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
    type = N_OPERATION;
}

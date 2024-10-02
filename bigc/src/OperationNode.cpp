#include "OperationNode.h"
#include "builtin.h"
#include "VariableNode.h"

Control OperationNode::resolve(State &state)
{
    if (children.empty())
        return Control("no operands");
    // do not do this for assignment
    if (op != ASS && op != CAST)
    {
        for (auto child : children)
        {
            Control control = child->resolve(state);
            if (control.control())
            {
                value = child->getValue(state);
                return control;
            }
            if (control.error())
                return control.stack("during operation:\n");
        }
    }
    Result<Value> result("failed to perform operation");
    Control control;
    std::string typeName;
    Wildcard val;
    switch (op)
    {
    case ASS:
        if (children.size() != 2)
            return Control("invalid number of operands for assignment: '" + std::to_string(children.size()) + '\'');
        // resolve right hand side of assignment operator
        control = children[1]->resolve(state);
        if (control.control())
        {
            value = children[1]->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("during assignment:\n");
        if (children[0]->getType() == N_IDENTIFIER && dynamic_cast<VariableNode *>(children[0]))
        {
            state.setVariable(((VariableNode *)children[0])->getVariable(), children[1]->getValue(state));
        }
        else
            return Control("cannot assign to non-identifier");
        value = children[1]->getValue(state);
        return Control(OK);
    case CAST:
        if (children.size() != 2)
            return Control("invalid number of operands for cast: '" + std::to_string(children.size()) + '\'');
        // ensure we have a variable node (type or identifier)
        if (!dynamic_cast<VariableNode *>(children[1]))
            return Control("second operand of cast is not a variable");
        // ensure the type name is actually a type
        typeName = (((VariableNode *)children[1])->getVariable());
        if (!state.isType(typeName))
            return Control("second operand of cast is not a type");
        // resolve the value
        control = children[0]->resolve(state);
        if (control.control())
        {
            value = children[0]->getValue(state);
            return control;
        }
        if (control.error())
            return control.stack("during operation:\n");
        // if the types are the same just return
        if (children[0]->getValue(state).getType() == typeName)
            return Control(OK);
        val = children[0]->getValue(state).getValue();
        if (bool *x = std::get_if<bool>(&val))
        {
            if (typeName == "int")
                value = Value((int)*x);
            else if (typeName == "long")
                value = Value((long)*x);
            else if (typeName == "char")
                value = Value((char)*x);
            else if (typeName == "float")
                value = Value(*x ? 1.0 : 0.0);
            else if (typeName == "double")
                value = Value((double)*x);
            else if (typeName == "str")
                value = Value(new std::string(*x ? "true" : "false"));
        }
        else if (int *x = std::get_if<int>(&val))
        {
            if (typeName == "bool")
                value = Value(*x != 0);
            else if (typeName == "long")
                value = Value((long)*x);
            else if (typeName == "char")
                value = Value((char)*x);
            else if (typeName == "float")
                value = Value((float)*x);
            else if (typeName == "double")
                value = Value((double)*x);
            else if (typeName == "str")
                value = Value(new std::string(std::to_string(*x)));
        }
        else if (long *x = std::get_if<long>(&val))
        {
            if (typeName == "bool")
                value = Value(*x != 0);
            else if (typeName == "int")
                value = Value((int)*x);
            else if (typeName == "char")
                value = Value((char)*x);
            else if (typeName == "float")
                value = Value((float)*x);
            else if (typeName == "double")
                value = Value((double)*x);
            else if (typeName == "str")
                value = Value(new std::string(std::to_string(*x)));
        }
        else if (char *x = std::get_if<char>(&val))
        {
            if (typeName == "bool")
                value = Value(*x != 0);
            else if (typeName == "int")
                value = Value((int)*x);
            else if (typeName == "long")
                value = Value((long)*x);
            else if (typeName == "float")
                value = Value((float)*x);
            else if (typeName == "double")
                value = Value((double)*x);
            else if (typeName == "str")
                value = Value(new std::string(std::to_string(*x)));
        }
        else if (float *x = std::get_if<float>(&val))
        {
            if (typeName == "bool")
                value = Value(*x != 0);
            else if (typeName == "int")
                value = Value((int)*x);
            else if (typeName == "long")
                value = Value((long)*x);
            else if (typeName == "char")
                value = Value((char)*x);
            else if (typeName == "double")
                value = Value((double)*x);
            else if (typeName == "str")
                value = Value(new std::string(std::to_string(*x)));
        }
        else if (double *x = std::get_if<double>(&val))
        {
            if (typeName == "bool")
                value = Value(*x != 0);
            else if (typeName == "int")
                value = Value((int)*x);
            else if (typeName == "long")
                value = Value((long)*x);
            else if (typeName == "char")
                value = Value((char)*x);
            else if (typeName == "float")
                value = Value((float)*x);
            else if (typeName == "str")
                value = Value(new std::string(std::to_string(*x)));
        }
        else if (std::string **x = std::get_if<std::string *>(&val))
        {
            if (typeName == "bool")
                value = Value(!((*x)->empty()));
            else if (typeName == "int")
            {
                try
                {
                    value = Value(std::stoi(**x));
                }
                catch (const std::exception &e)
                {
                    return Control("cannot cast str to int");
                }
            }
            else if (typeName == "long")
            {
                try
                {
                    value = Value(std::stol(**x));
                }
                catch (const std::exception &e)
                {
                    return Control("cannot cast str to long");
                }
            }
            else if (typeName == "char")
            {
                if ((*x)->length() != 1)
                    return Control("cannot cast str with multiple chars to char");
                value = Value((**x)[0]);
            }
            else if (typeName == "float")
            {
                try
                {
                    value = Value(std::stof(**x));
                }
                catch (const std::exception &e)
                {
                    return Control("cannot cast str to float");
                }
            }
            else if (typeName == "double")
            {
                try
                {
                    value = Value(std::stod(**x));
                }
                catch (const std::exception &e)
                {
                    return Control("cannot cast str to double");
                }
            }
        }
        else
            return "cannot cast " + children[0]->getValue(state).getType() + " to " + typeName;
        return Control(OK);
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
        return Control("no operator specified");
    }
    if (!result.ok())
        return "during operation:\n" + result.getError();
    value = result.getValue();
    return Control(OK);
}

OperationNode::OperationNode(Token &token)
{
    if (token.type != OPERATOR)
        throw "rip";
    if (token.value == "=")
        op = ASS;
    else if (token.value == "@")
        op = CAST;
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

#include "Value.h"
#include <algorithm>

Value::Value()
{
}

Value::Value(Token &token)
{
    switch (token.type)
    {
    case NUMBERSTR:
        switch (token.value[0])
        {
        case 's':
            value = new std::string(token.value.substr(1));
        case 'n':
            value = new int(std::atoi(token.value.substr(1).c_str()));
            break;
        default:
            std::cout << "that's not a valid numberstr\n";
            throw "unrecognized starting character for value";
        }
        break;
    default:
        std::cout << "invalid token type for a value Node\n";
        throw;
    }
}

Value::Value(Wildcard value)
{
    this->value = value;
}

Wildcard Value::getValue()
{
    return value;
}

std::string Value::getType()
{
    if (std::holds_alternative<int *>(value))
        return "int";
    else if (std::holds_alternative<float *>(value))
        return "float";
    else if (std::holds_alternative<char *>(value))
        return "char";
    else if (std::holds_alternative<std::string *>(value))
        return "str";
    else if (std::holds_alternative<Iterable<Value> *>(value))
        return "arr";
    else if (std::holds_alternative<Node *>(value))
        return "fn";
    else if (std::holds_alternative<Object *>(value))
        return "obj";
    return "nil";
}

Result<Value> Value::add(Value other)
{
    Wildcard val;
    Wildcard otherVal = other.getValue();
    // int
    if (int **x = std::get_if<int *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new int(**x + **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x + **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new int(**x + **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x + **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x + **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot add int to " + getType());
    }
    // long
    else if (long **x = std::get_if<long *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new long(**x + **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x + **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new long(**x + **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x + **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x + **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot add long to " + getType());
    }
    // char
    else if (char **x = std::get_if<char *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new int(**x + **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x + **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new char(**x + **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x + **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x + **y);
            return Result<Value>(val);
        }
        else if (std::string **y = std::get_if<std::string *>(&value))
        {
            val = new std::string(**x + **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot add char to " + getType());
    }
    // float
    else if (float **x = std::get_if<float *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new float(**x + **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new float(**x + **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new float(**x + **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x + **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x + **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot add float to " + getType());
    }
    // double
    else if (double **x = std::get_if<double *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new double(**x + **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new double(**x + **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new double(**x + **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new double(**x + **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x + **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot add double to " + getType());
    }
    // str
    else if (std::string **x = std::get_if<std::string *>(&otherVal))
    {
        if (char **y = std::get_if<char *>(&value))
        {
            val = new std::string(**x + **y);
            return Result<Value>(val);
        }
        else if (std::string **x = std::get_if<std::string *>(&value))
        {
            val = new std::string(**x + **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot add str to " + getType());
    }
    // arr
    else if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&otherVal))
    {
        if (Iterable<Value> **y = std::get_if<Iterable<Value> *>(&value))
        {

            auto result = ((*x)->combine(**y));
            if (!result.ok())
                return Result<Value>(result.getError());
            val = result.getValue();
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot add arr to " + getType());
    }
    else
        return Result<Value>("cannot add " + other.getType() + " to " + getType());
    return Result<Value>("failed to add");
}

Result<Value> Value::inverse()
{
    Wildcard val;
    if (int **x = std::get_if<int *>(&value))
    {
        val = new int(-**x);
        return Result<Value>(val);
    }
    else if (long **x = std::get_if<long *>(&value))
    {
        val = new long(-**x);
        return Result<Value>(val);
    }
    else if (char **x = std::get_if<char *>(&value))
    {
        val = new char(-**x);
        return Result<Value>(val);
    }
    else if (float **x = std::get_if<float *>(&value))
    {
        val = new float(-**x);
        return Result<Value>(val);
    }
    else if (double **x = std::get_if<double *>(&value))
    {
        val = new double(-**x);
        return Result<Value>(val);
    }
    else if (std::string **x = std::get_if<std::string *>(&value))
    {
        std::string *newStr = new std::string(**x);
        std::reverse(newStr->begin(), newStr->end());
        val = newStr;
        return Result<Value>(val);
    }
    else
        return Result<Value>("cannot negate " + getType());
    return Result<Value>("failed to negate");
}

Result<Value> Value::subtract(Value other)
{
    Wildcard otherVal = other.getValue();
    Wildcard val;
    // int
    if (int **x = std::get_if<int *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new int(**x - **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x - **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new int(**x - **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x - **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x - **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot subtract int from " + getType());
    }
    // long
    else if (long **x = std::get_if<long *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new long(**x - **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x - **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new long(**x - **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x - **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x - **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot add long to " + getType());
    }
    // char
    else if (char **x = std::get_if<char *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new int(**x - **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x - **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new char(**x - **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x - **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x - **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot subtract char from " + getType());
    }
    // float
    else if (float **x = std::get_if<float *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new float(**x - **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new float(**x - **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new float(**x - **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x - **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x - **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot subtract float from " + getType());
    }
    // double
    else if (double **x = std::get_if<double *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new double(**x - **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new double(**x - **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new double(**x - **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new double(**x - **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x - **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot subtract double from " + getType());
    }
    else
        return Result<Value>("cannot subtract " + other.getType() + " to " + getType());
    return Result<Value>("failed to subtract");
}

Result<Value> Value::multiply(Value other)
{
    Wildcard val;
    Wildcard otherVal = other.getValue();
    // int
    if (int **x = std::get_if<int *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new int(**x * **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x * **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new int(**x * **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x * **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x * **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot multiply int with " + getType());
    }
    // long
    else if (long **x = std::get_if<long *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new long(**x * **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x * **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new long(**x * **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x * **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x * **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot multiply long with " + getType());
    }
    // char
    else if (char **x = std::get_if<char *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new int(**x * **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x * **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new char(**x * **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x * **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x * **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot multiply char with " + getType());
    }
    // float
    else if (float **x = std::get_if<float *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new float(**x * **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new float(**x * **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new float(**x * **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x * **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x * **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot multiply float with " + getType());
    }
    // double
    else if (double **x = std::get_if<double *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new double(**x * **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new double(**x * **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new double(**x * **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new double(**x * **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x * **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot multiply double with " + getType());
    }
    else
        return Result<Value>("cannot multiply " + other.getType() + " with " + getType());

    return Result<Value>("failed to multiply");
}

Result<Value> Value::reciprocate()
{
    Wildcard val;
    if (int **x = std::get_if<int *>(&value))
    {
        val = new int(1 / **x);
        return Result<Value>(val);
    }
    else if (long **x = std::get_if<long *>(&value))
    {
        val = new long(1 / **x);
        return Result<Value>(val);
    }
    else if (char **x = std::get_if<char *>(&value))
    {
        val = new char(1 / **x);
        return Result<Value>(val);
    }
    else if (float **x = std::get_if<float *>(&value))
    {
        val = new float(1 / **x);
        return Result<Value>(val);
    }
    else if (double **x = std::get_if<double *>(&value))
    {
        val = new double(1 / **x);
        return Result<Value>(val);
    }
    else
        return Result<Value>("cannot reciprocate " + getType());
    return Result<Value>("failed to reciprocate");
}

Result<Value> Value::divide(Value other)
{
    Wildcard val;
    Wildcard otherVal = other.getValue();
    // int
    if (int **x = std::get_if<int *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new int(**x / **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x / **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new int(**x / **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x / **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x / **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot divide int with " + getType());
    }
    // long
    else if (long **x = std::get_if<long *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new long(**x / **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x / **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new long(**x / **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x / **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x / **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot divide long with " + getType());
    }
    // char
    else if (char **x = std::get_if<char *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new int(**x / **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x / **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new char(**x / **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x / **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x / **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot divide char with " + getType());
    }
    // float
    else if (float **x = std::get_if<float *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new float(**x / **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new float(**x / **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new float(**x / **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new float(**x / **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x / **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot divide float with " + getType());
    }
    // double
    else if (double **x = std::get_if<double *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new double(**x / **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new double(**x / **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new double(**x / **y);
            return Result<Value>(val);
        }
        else if (float **y = std::get_if<float *>(&value))
        {
            val = new double(**x / **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new double(**x / **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot divide double with " + getType());
    }
    else
        return Result<Value>("cannot divide " + other.getType() + " with " + getType());
    return Result<Value>("failed to divide");
}

Result<Value> Value::modulo(Value other)
{
    Wildcard otherVal = other.getValue();
    Wildcard val;
    if (int **x = std::get_if<int *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new int(**x % **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x % **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new int(**x % **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot modulo int with " + getType());
    }
    else if (long **x = std::get_if<long *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new long(**x % **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x % **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new long(**x % **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot modulo long with " + getType());
    }
    else if (char **x = std::get_if<char *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new int(**x % **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new long(**x % **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new char(**x % **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot modulo char with " + getType());
    }
    else
        return Result<Value>("cannot modulo " + other.getType() + " with " + getType());
    return Result<Value>("failed to modulo");
}

Result<Value> Value::isEqual(Value other)
{
    Wildcard val;
    Wildcard otherVal = other.getValue();
    // int
    if (int **x = std::get_if<int *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot determine equality of int with " + getType());
    }
    else if (long **x = std::get_if<long *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot determine equality of long with " + getType());
    }
    else if (char **x = std::get_if<char *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot determine equality of char with " + getType());
    }
    else if (float **x = std::get_if<float *>(&otherVal))
    {
        if (float **y = std::get_if<float *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
    }
    else if (double **x = std::get_if<double *>(&otherVal))
    {
        if (float **y = std::get_if<float *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
    }
    else if (std::string **x = std::get_if<std::string *>(&otherVal))
    {
        if (std::string **y = std::get_if<std::string *>(&value))
        {
            val = new bool(**x == **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot determine equality of str with " + getType());
    }
    else
        return Result<Value>("cannot determine equality of " + other.getType() + " with " + getType());
    return Result<Value>("failed to determine equality");
}

Result<Value> Value::isLessThan(Value other)
{
    Wildcard otherVal = other.getValue();
    Wildcard val;
    // int
    if (int **x = std::get_if<int *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot do comparison of int with " + getType());
    }
    else if (long **x = std::get_if<long *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot do comparison of long with " + getType());
    }
    else if (char **x = std::get_if<char *>(&otherVal))
    {
        if (int **y = std::get_if<int *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else if (long **y = std::get_if<long *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else if (char **y = std::get_if<char *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot do comparison of char with " + getType());
    }
    else if (float **x = std::get_if<float *>(&otherVal))
    {
        if (float **y = std::get_if<float *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot do comparison of float with " + getType());
    }
    else if (double **x = std::get_if<double *>(&otherVal))
    {
        if (float **y = std::get_if<float *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else if (double **y = std::get_if<double *>(&value))
        {
            val = new bool(**x < **y);
            return Result<Value>(val);
        }
        else
            return Result<Value>("cannot do comparison of double with " + getType());
    }
    else
        return Result<Value>("cannot do comparison of " + other.getType() + " with " + getType());
    return Result<Value>("failed to do comparison");
}

Result<Value> Value::isNotEqual(Value other)
{
    Wildcard val;
    Result<Value> equalityResult = this->isEqual(other);
    Wildcard equality = equalityResult.getValue().getValue();
    if (!equalityResult.ok())
        return equalityResult.getError();
    bool **isEqual = std::get_if<bool *>(&equality);
    val = new bool(!**isEqual);
    return Result<Value>(val);
}

Result<Value> Value::isGreaterThan(Value other)
{
    Wildcard val;
    bool isEqual;
    bool isLessThan;
    Result<Value> equalityResult = this->isEqual(other);
    Result<Value> lessThanResult = this->isLessThan(other);
    if (!equalityResult.ok() && !lessThanResult.ok())
        return lessThanResult.getError();
    if (equalityResult.ok())
    {
        auto equal = equalityResult.getValue().getValue();
        isEqual = **std::get_if<bool *>(&equal);
    }
    else
        isEqual = false;
    if (lessThanResult.ok())
    {
        auto lessThan = lessThanResult.getValue().getValue();
        isLessThan = std::get_if<bool *>(&lessThan);
    }
    else
        isLessThan = false;
    val = new bool(isEqual || isLessThan);
    return Result<Value>(val);
}

Result<Value> Value::negate()
{
    return Result<Value>("failed to negate");
}

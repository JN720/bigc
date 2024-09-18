#include "builtin.h"
#include "FunctionNode.h"

namespace base
{

    void printValue(Wildcard val)
    {
        if (bool *x = std::get_if<bool>(&val))
            std::cout << ((*x) ? "true" : "false");
        else if (int *x = std::get_if<int>(&val))
            std::cout << *x;
        else if (long *x = std::get_if<long>(&val))
            std::cout << *x;
        else if (float *x = std::get_if<float>(&val))
            std::cout << *x;
        else if (double *x = std::get_if<double>(&val))
            std::cout << *x;
        else if (char *x = std::get_if<char>(&val))
            std::cout << *x;
        else if (std::string **x = std::get_if<std::string *>(&val))
            std::cout << **x;
        else if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
            std::cout << "arr";
        else if (Node **x = std::get_if<Node *>(&val))
            std::cout << "<function>";
    }

    Result<Value> print(State &state, std::vector<Node *> &args)
    {
        std::string printout;
        for (Node *arg : args)
        {
            Wildcard val = arg->getValue(state).getValue();
            while (Object **x = std::get_if<Object *>(&val))
            {
                if (state.implements(arg->getValue(state).getType(), "Printable"))
                {
                    Result<Node *> result = state.getClassMethod(arg->getValue(state).getType(), "toString");
                    if (!result.ok())
                        return Result<Value>("making printable:\n" + result.getError());
                    FunctionNode *function = (FunctionNode *)result.getValue();
                    if (!function)
                        return Result<Value>("invalid function");
                    Result<Value> output = function->execute(state, args);
                    if (!output.ok())
                        return Result<Value>(output.getError());
                    val = output.getValue().getValue();
                }
            }
            printValue(val);
        }
        return Result<Value>();
    }

    void println(State &state, std::vector<Node *> &args)
    {
        print(state, args);
        std::cout << '\n';
    }

    Result<Value> len(State &state, std::vector<Node *> &args)
    {
        if (args.size() != 1)
            return Result<Value>("invalid arity");
        Value arg = args[0]->getValue(state);
        Wildcard val = arg.getValue();
        if (arg.getType() == "nil")
            return Result<Value>(0);
        else if (std::string **x = std::get_if<std::string *>(&val))
            return Result<Value>(Value((int)(*x)->length()));
        else if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
        {
            Result<int> length = (*x)->len();
            if (!length.ok())
                return Result<Value>("getting length:\n" + length.getError());
            return Result<Value>(Value(length.getValue()));
        }
        else if (Object **x = std::get_if<Object *>(&val))
        {
            if (state.implements(arg.getType(), "Measurable"))
            {
                Result<Node *> result = state.getClassMethod(arg.getType(), "len");
                if (!result.ok())
                    return Result<Value>("getting length:\n" + result.getError());
                FunctionNode *function = (FunctionNode *)result.getValue();
                if (!function)
                    return Result<Value>("invalid function");
                Result<Value> output = function->execute(state, args);
                if (!output.ok())
                    return Result<Value>("executing len:\n" + output.getError());
                return Result<Value>(output.getValue());
            }
            else
                return Result<Value>("not measurable");
        }
        return Result<Value>("not iterable");
    }

    Result<Value> input(State &state, std::vector<Node *> &args)
    {
        return Result<Value>();
    }

    void debugPrint(Value value)
    {
        std::cout << "type: " << value.getType() << ' ' << '\n';
        Wildcard val = value.getValue();
        if (value.getType() == "nil")
        {
            std::cout << "nil\n";
        }
        else if (bool *x = std::get_if<bool>(&val))
            std::cout << ((*x) ? "true" : "false") << '\n';
        else if (int *x = std::get_if<int>(&val))
            std::cout << *x << '\n';
        else if (long *x = std::get_if<long>(&val))
            std::cout << *x << '\n';
        else if (float *x = std::get_if<float>(&val))
            std::cout << *x << '\n';
        else if (double *x = std::get_if<double>(&val))
            std::cout << *x << '\n';
        else if (char *x = std::get_if<char>(&val))
            std::cout << *x << '\n';
        else if (std::string **x = std::get_if<std::string *>(&val))
            std::cout << **x << '\n';
        else if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
            std::cout << "arr" << '\n';
        else
            std::cout << "no print implemented" << '\n';
    }

    Result<Value> executeFundamentalFunction(int index, State &state, std::vector<Node *> &args)
    {
        Result<Value> result;
        switch (index)
        {
        case 0:
            result = base::print(state, args);
            break;
        case 1:
            base::println(state, args);
            break;
        case 2:
            result = base::len(state, args);
            break;
        case 4:
            result = base::input(state, args);
        }
        return Result<Value>("failed to execute fundamental function");
    }
};

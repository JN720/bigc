#include "builtin.h"
#include "FunctionNode.h"

namespace base
{
    std::string fmtValue(Wildcard val)
    {
        if (bool *x = std::get_if<bool>(&val))
            return ((*x) ? "true" : "false");
        else if (int *x = std::get_if<int>(&val))
            return std::to_string(*x);
        else if (long *x = std::get_if<long>(&val))
            return std::to_string(*x);
        else if (float *x = std::get_if<float>(&val))
            return std::to_string(*x);
        else if (double *x = std::get_if<double>(&val))
            return std::to_string(*x);
        else if (char *x = std::get_if<char>(&val))
            return std::to_string(*x);
        else if (std::string **x = std::get_if<std::string *>(&val))
            return **x;
        else if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
            return "arr";
        else if (Node **x = std::get_if<Node *>(&val))
            return "<function>";
        return "nil";
    }

    void printValue(Wildcard val)
    {
        std::cout << fmtValue(val) << '\n';
    }

    Result<Value> print(State &state, std::vector<Node *> &args)
    {
        std::string printout;
        for (Node *arg : args)
        {
            Wildcard val = arg->getValue(state).getValue();
            // for an object keep executing toString until we get a non-obj
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
            printout += fmtValue(val);
            if (arg != args.back())
                printout += ' ';
        }
        std::cout << printout;
        return Result<Value>(Value(new std::string(printout)));
    }

    Result<Value> println(State &state, std::vector<Node *> &args)
    {
        Result<Value> result = print(state, args);
        std::cout << '\n';
        return result;
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

    Result<Value> type(State &state, std::vector<Node *> &args)
    {
        if (args.size() != 1)
            return Result<Value>("invalid arity");
        return Result<Value>(Value(new std::string(args[0]->getValue(state).getType())));
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
            return base::print(state, args);
        case 1:
            return base::println(state, args);
        case 2:
            return base::len(state, args);
        case 3:
            return base::type(state, args);
        case 4:
            result = base::input(state, args);
        }
        return Result<Value>("failed to execute fundamental function");
    }
};

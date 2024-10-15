#include "builtin.h"
#include "FunctionNode.h"
#include "InterfaceNode.h"
#include "ClassDefinition.h"

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
        else if (Object **x = std::get_if<Object *>(&val))
            return "<object>";
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
            while (Object **obj = std::get_if<Object *>(&val))
            {
                // get the interface from the state
                Result<Value> interfaceResult = state.getVariable("Printable");
                if (!interfaceResult.ok())
                    return Result<Value>("making Printable:\n" + interfaceResult.getError());
                Wildcard interfaceVal = interfaceResult.getValue().getValue();
                Interface *printInterface;
                if (Node **x = std::get_if<Node *>(&interfaceVal))
                {
                    if (InterfaceNode *printInterfaceNode = dynamic_cast<InterfaceNode *>(*x))
                    {
                        printInterface = printInterfaceNode->getInterface();
                    }
                    else
                        return Result<Value>("Printable is not an interface");
                }
                else
                    return Result<Value>("Printable is not a node");
                // check if the obj implements it
                ClassDefinition *objClass = (*obj)->getClass();
                if (objClass->implements(printInterface))
                {
                    // grab the method
                    Result<Node *> result = objClass->getClassMethod("toString");
                    if (!result.ok())
                        return Result<Value>("making Printable:\n" + result.getError());
                    FunctionNode *function = (FunctionNode *)result.getValue();
                    if (!function)
                        return Result<Value>("invalid function");
                    // execute with the object
                    Result<Value> output = function->executeInstanced(*obj, &state, args);
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
        else if (Object **obj = std::get_if<Object *>(&val))
        {
            Result<Value> interfaceResult = state.getVariable("Measurable");
            if (!interfaceResult.ok())
                return Result<Value>("making Measurable:\n" + interfaceResult.getError());
            Wildcard interfaceVal = interfaceResult.getValue().getValue();
            Interface *measureInterface;
            if (Node **x = std::get_if<Node *>(&interfaceVal))
            {
                if (InterfaceNode *measureInterfaceNode = dynamic_cast<InterfaceNode *>(*x))
                {
                    measureInterface = measureInterfaceNode->getInterface();
                }
                else
                    return Result<Value>("Measurable is not an interface");
            }
            else
                return Result<Value>("Measurable is not a node");

            ClassDefinition *objClass = (*obj)->getClass();
            if (objClass->implements(measureInterface))
            {
                Result<Node *> result = objClass->getClassMethod("len");
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
        std::string input;
        std::cin >> input;
        return Result<Value>(Value(new std::string(input)));
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
        else if (Object **x = std::get_if<Object *>(&val))
            std::cout << "obj" << '\n';
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
            return base::input(state, args);
        }
        return Result<Value>("failed to execute fundamental function");
    }
};

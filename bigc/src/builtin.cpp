#include "builtin.h"
#include "ast.h"
#include "MethodNode.h"
#include "InterfaceNode.h"
#include "LibraryNode.h"
#include "ClassDefinition.h"
#include "./Libraries/Math/MathLibrary.h"
#include "./Libraries/File/FileLibrary.h"
#include "./Libraries/OS/OSLibrary.h"
#include "./Libraries/Thread/ThreadLibrary.h"
#include "./Libraries/Time/TimeLibrary.h"
#include "./Libraries/Net/NetLibrary.h"
#include <fstream>
namespace base
{
    std::string fmtValue(Wildcard val)
    {
        Node **asNode = std::get_if<Node *>(&val);
        if (asNode && *asNode == nullptr)
            return "nil";
        else if (bool *x = std::get_if<bool>(&val))
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

    void printValue(Wildcard val, State &state)
    {
        state.log(fmtValue(val) + '\n');
    }

    Result<Value> print(State &state, std::vector<Node *> &args)
    {
        std::string printout;
        for (Node *arg : args)
        {
            Control control = arg->resolve(state);
            if (control.control())
                return Result<Value>(control);
            if (control.error())
                return Result<Value>(control.stack("resolving arguments:\n"));
            Wildcard val = arg->getValue(state).getValue();
            // for an object keep executing toString until we get a non-obj
            while (Object **obj = std::get_if<Object *>(&val))
            {
                if (!*obj)
                    return Result<Value>("invalid object");
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
                ClassDefinition *objClass = static_cast<ClassDefinition *>((*obj)->getClass());
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
                    MethodNode *method = new MethodNode(function, *obj, state);
                    Result<Value> output = method->execute(state, args);
                    if (!output.ok())
                        return Result<Value>(output.getError());
                    val = output.getValue().getValue();
                }
            }
            printout += fmtValue(val);
            if (arg != args.back())
                printout += ' ';
        }
        state.log(printout);
        return Result<Value>(Value(new std::string(printout)));
    }

    Result<Value> println(State &state, std::vector<Node *> &args)
    {
        Result<Value> result = print(state, args);
        state.log("\n");
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

            ClassDefinition *objClass = static_cast<ClassDefinition *>((*obj)->getClass());
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
        std::string input = state.input();
        return Result<Value>(Value(new std::string(input)));
    }
    // imported groups
    Result<Value> library(State &state, std::vector<Node *> &args)
    {
        if (args.size() != 1)
            return Result<Value>("invalid arity");
        Wildcard val = args[0]->getValue(state).getValue();
        std::string *path;
        if (std::string **x = std::get_if<std::string *>(&val))
            path = *x;
        else
            return Result<Value>("invalid path");
        Result<State> result = ast::evaluate(*path);
        if (!result.ok())
            return Result<Value>(result.getError());
        State libState = result.getValue();
        return Result<Value>(Value(new LibraryNode(libState.getRegistry())));
    }
    // built-in/installed libraries
    Result<Value> include(State &state, std::vector<Node *> &args)
    {
        if (args.size() != 1)
            return Result<Value>("invalid arity");
        Wildcard val = args[0]->getValue(state).getValue();
        std::string *library;
        if (std::string **x = std::get_if<std::string *>(&val))
            library = *x;
        else
            return Result<Value>("invalid library");
        if (*library == "math")
            return Result<Value>(Value((Node *)new LibraryNode(libmath::init())));
#if defined(BUILD_CPU) || defined(BUILD_GPU)
        else if (*library == "file")
            return Result<Value>(Value((Node *)new LibraryNode(libfile::init())));
        else if (*library == "os")
            return Result<Value>(Value((Node *)new LibraryNode(libos::init())));
        else if (*library == "net")
            return Result<Value>(Value((Node *)new LibraryNode(libnet::init())));
#endif
        else if (*library == "thread")
            return Result<Value>(Value((Node *)new LibraryNode(libthread::init())));
        else if (*library == "time")
            return Result<Value>(Value((Node *)new LibraryNode(libtime::init())));
        return Result<Value>("library not found");
    }
    // imported files
    Result<Value> import(State &state, std::vector<Node *> &args)
    {
        if (args.size() != 1)
            return Result<Value>("invalid arity");
        Wildcard val = args[0]->getValue(state).getValue();
        std::string *path;
        if (std::string **x = std::get_if<std::string *>(&val))
            path = *x;
        else
            return Result<Value>("invalid path");
        Result<State> result = ast::evaluate(*path);
        if (!result.ok())
            return Result<Value>(result.getError());
        State libState = result.getValue();
        return Result<Value>(Value(new LibraryNode(libState.getRegistry())));
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
        case 5:
            return base::import(state, args);
        case 6:
            return base::include(state, args);
        }
        return Result<Value>("failed to execute fundamental function");
    }
};

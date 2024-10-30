#include "interpreter.h"

#include <fstream>
#include <iostream>
#include "builtin.h"
#include "InterfaceNode.h"
#include "AccessNode.h"
#include "FunctionNode.h"
#include "ClassNode.h"

void interpreter::addBasics(State &state)
{
    state.registerVariable("true", Value(true));
    state.registerVariable("false", Value(false));

    InterfaceNode *callable = new InterfaceNode();
    callable->addChild(new IdentifierNode("call"));
    state.setVariable("Callable", Value(callable));

    InterfaceNode *printable = new InterfaceNode();
    printable->addChild(new IdentifierNode("toString"));
    state.setVariable("Printable", Value(printable));

    ClassDefinition *exceptionDefinition = new ClassDefinition();
    FunctionNode *exceptionErrorFunction = new FunctionNode();
    SequenceNode *exceptionErrorSequence = new SequenceNode();
    AccessNode *errorPropertyAccess = new AccessNode("message");
    errorPropertyAccess->addChild(new IdentifierNode("this"));
    exceptionErrorSequence->addChild(errorPropertyAccess);
    exceptionErrorFunction->addChild(exceptionErrorSequence);
    exceptionDefinition->addMethod("error", exceptionErrorFunction, false, PUBLIC);
    exceptionDefinition->addAttribute("message", PROTECTED, false, Value(new std::string("Unknown Error")));

    Token equalToken("=", OPERATOR);
    FunctionNode *exceptionConstructor = new FunctionNode();
    SequenceNode *exceptionConstructorSequence = new SequenceNode();
    OperationNode *exceptionErrorAssignment = new OperationNode(equalToken);
    AccessNode *errorPropertyAccessConstructor = new AccessNode("message");
    errorPropertyAccessConstructor->addChild(new IdentifierNode("this"));
    exceptionErrorAssignment->addChild(errorPropertyAccess);
    exceptionErrorAssignment->addChild(new IdentifierNode("error"));
    exceptionConstructor->addChild(new IdentifierNode("error"));
    exceptionConstructor->addChild(exceptionErrorAssignment);
    exceptionDefinition->addMethod("constructor", exceptionConstructor, false, PUBLIC);
    state.setVariable("Exception", Value(new ClassNode(exceptionDefinition)));
}

void interpreter::interpretFile(const std::string &filename)
{
    std::ifstream file;
    try
    {
        file = std::ifstream(filename);
    }
    catch (...)
    {
        std::cout << "failed to find file";
        return;
    }
    std::string programText;
    std::string line;
    while (std::getline(file, line))
    {
        programText += line + '\n';
    }
    Result<std::vector<Token>> result = tokenizer::tokenize(programText);

    if (!result.ok())
    {
        std::cout << "Tokenization Error: " << result.getError() << '\n';
        return;
    }
    std::cout << "token count: " << result.getValue().size() << '\n';
    SequenceNode *program = new SequenceNode();
    int index = 0;
    auto tokens = result.getValue();
    State state;
    addBasics(state);
    std::string error = ast::createAST(state, tokens, index, program, ast::BASE, false);
    if (!error.empty())
    {
        std::cout << "Interpretation Error: " << error << '\n';
        ast::printTree(*program, 0);
        return;
    }
    std::cout << "\n";
    ast::printTree(*program, 0);
    std::cout << "\n";
    Control control = program->resolve(state);
    if (control.error())
    {
        std::cout << "Runtime Error:\n"
                  << control.getError() << '\n';
        return;
    }
}

void interpreter::debugRepl()
{
    State state;
    addBasics(state);
    std::string line = "";

    std::cout << "Welcome to the Big C Lemur Engine!\n\n";
    while (true)
    {
        std::cout << ">> ";
        std::cin >> line;
        if (line == "exit")
            break;
        Result<std::vector<Token>> result = tokenizer::tokenize(line);

        if (!result.ok())
        {
            std::cout << "Tokenization Error: " << result.getError() << '\n';
            continue;
        }
        std::cout << "token count: " << result.getValue().size() << '\n';
        SequenceNode *program = new SequenceNode();
        int index = 0;
        auto tokens = result.getValue();
        std::string error = ast::createAST(state, tokens, index, program, ast::BASE, false);
        if (!error.empty())
        {
            std::cout << "Interpretation Error: " << error << '\n';
            ast::printTree(*program, 0);
            continue;
        }
        std::cout << "\n";
        ast::printTree(*program, 0);
        std::cout << "\n";
        Control control = program->resolve(state);
        if (control.error())
        {
            std::cout << "Runtime Error:\n"
                      << control.getError() << '\n';
            continue;
        }
        Value value = program->getChildren().back()->getValue(state);
        Wildcard valueValue = value.getValue();
        std::cout << "type: " << value.getType() << ' ' << '\n';
        base::debugPrint(value);
    }
}

void interpreter::repl()
{
    State state;
    std::string line = "";

    std::cout << "Welcome to the Big C Lemur Engine!\n\n";
    while (true)
    {
        std::cout << ">> ";
        std::cin >> line;
        if (line == "exit")
            break;
        Result<std::vector<Token>> result = tokenizer::tokenize(line);

        if (!result.ok())
        {
            std::cout << "Tokenization Error: " << result.getError() << '\n';
            continue;
        }
        SequenceNode *program = new SequenceNode();
        int index = 0;
        auto tokens = result.getValue();
        std::string error = ast::createAST(state, tokens, index, program, ast::BASE, false);
        if (!error.empty())
        {
            std::cout << "Interpretation Error: " << error << '\n';
            continue;
        }
        Control control = program->resolve(state);
        if (control.error())
        {
            std::cout << "Runtime Error:\n"
                      << control.getError() << '\n';
        }
    }
}

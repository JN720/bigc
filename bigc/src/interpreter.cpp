#include "interpreter.h"

#include <fstream>
#include <iostream>

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
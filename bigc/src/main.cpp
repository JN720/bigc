#include <iostream>
#include "Expression.h"
#include <stack>
#include <vector>
#include "Token.h"

const std::string OPERATORS = "+-*/";
const std::string DIGITS = "0123456789";
const std::string VARS = "abcdefghijklmnopqrstuvwxyz";

Token interpretExpr(std::string expr, int &index)
{
    std::vector<Token> tokens;
    std::string accumulatedDigits = "";
    std::string accumulatedValue = "";
    for (int i = index; i < expr.length(); i++)
    {
        char c = expr[i];
        if (DIGITS.find(c) != std::string::npos)
        {
            if (!accumulatedValue.empty())
            {
                tokens.push_back(Token(accumulatedValue, VARIABLE));
                accumulatedValue = "";
            }
            accumulatedDigits += c;
        }
        else if (OPERATORS.find(c) != std::string::npos)
        {
            if (!accumulatedDigits.empty())
            {
                tokens.push_back(Token(accumulatedDigits, NUMBER));
                accumulatedDigits = "";
            }
            else if (!accumulatedValue.empty())
            {
                tokens.push_back(Token(accumulatedValue, VARIABLE));
                accumulatedValue = "";
            }
            tokens.push_back(Token(std::string(1, c), OPERATOR));
        }
        else if (VARS.find(c) != std::string::npos)
        {
            if (!accumulatedDigits.empty())
            {
                tokens.push_back(Token(accumulatedDigits, NUMBER));
            }
            accumulatedValue += c;
        }
        else if (c == '(')
        {
            tokens.push_back(interpretExpr(expr, ++i));
        }
        else if (c == ')')
        {
            i++;
            break;
        }
    }
    if (!accumulatedDigits.empty())
    {
        tokens.push_back(Token(accumulatedDigits, NUMBER));
    }
    if (accumulatedValue.empty())
    {
        tokens.push_back(Token(accumulatedValue, VARIABLE));
    }

    for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens[i].value << '\n';
    }
    return tokens[0];
}

int main(int argc, char *argv[])
{
    std::string line = "";
    for (int i = 1; i < argc; i++)
    {
        line += argv[i];
        line += " ";
    }
    int index = 0;
    interpretExpr(line, index);
    return 0;
}
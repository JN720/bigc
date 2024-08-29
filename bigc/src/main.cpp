#include <iostream>
#include <stack>
#include <vector>
#include "Token.h"
#include "OperationNode.h"
#include "IdentifierNode.h"
#include "IndexNode.h"
#include "AccessNode.h"
#include "BranchNode.h"

const std::string OPERATORS = "+-*!/=><";
const std::string DIGITS = "0123456789";
const std::string WORDS = "abcdefghijklmnopqrstuvwxyz_";
const std::string SINGULARS = ".,()[]{}|&~;";
const std::string KEYWORDS[] = {"if"};

template <class T>
struct Result
{
    T value;
    std::string error;

    Result<T>(T val)
    {
        value = val;
        error = "";
    }

    Result<T>(std::string str)
    {
        error = str;
    }
};

Result<TokenType> allowSingularTokens(char c, TokenType allowed[12], std::string error)
{
    for (int i = 0; i < 12; i++)
    {
        char token = SINGULARS[i];
        if (allowed[i] != NONE && c == token)
        {
            return allowed[i];
        }
    }
    return Result<TokenType>(error);
}

/* POST-CHARACTER MATRIX: x means disallowed
            a 0 + . , ( ) [ ] { } | & ~ ;
NONE              x x   x   x x x x x   x
TEXT          x                     x x
NUMBER      x x   x   x   x       x x x
ACCESSOR      x x x x x x x x x x x x x x
DELIMITER         x x   x   x x x x     x
ARGEXSTART        x x       x x x x     x
ARGEXEND    x x                     x x
INDSTART          x x   x   x x x x     x
INDEND      x x                     x
CTRLSTART         x x   x   x x x x     x
CTRLEND     x x                     x x
PIPE              x x   x   x x x x     x
PIPERES     x x                       x
SPREAD        x x x x   x   x x x x     x

*/

Result<TokenType> getTokenType(char c, TokenType prev, std::string acc, bool prevWhitespace)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return Result<TokenType>(NONE);
    bool hasDot = acc.find('.') != std::string::npos;
    TokenType allowed[12] = {
        ACCESSOR,
        DELIMITER,
        ARGEXPRSTART,
        ARGEXPREND,
        INDSTART,
        INDEND,
        CTRLSTART,
        CTRLEND,
        PIPE,
        PIPERES,
        SPREAD,
        END};

    switch (prev)
    {
    case NONE:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ACCESSOR - 4] = NONE;
        allowed[DELIMITER - 4] = NONE;
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLSTART - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[PIPERES - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected start of statement");

    case NUMBER:
        if (c == '.' && hasDot)
            return Result<TokenType>("why dot");
        if (DIGITS.find(c) != std::string::npos || (!hasDot && c == '.'))
            return Result<TokenType>(NUMBER);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[ARGEXPRSTART - 4] = NONE;
        allowed[INDSTART - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[PIPERES - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "invalid token after number");
    case TEXT:
        if (prevWhitespace && DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos || DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[PIPERES - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "invalid token after word");
    case OPERATOR:
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLSTART - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        return allowSingularTokens(c, allowed, "invalid token after operator");
    case ACCESSOR:
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        return Result<TokenType>("expected property");
    case DELIMITER:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ACCESSOR - 4] = NONE;
        allowed[DELIMITER - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLSTART - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "expected expression");
    case ARGEXPRSTART:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ACCESSOR - 4] = NONE;
        allowed[DELIMITER - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLSTART - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "expected value");
    case ARGEXPREND:
        /*if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);*/
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[PIPERES - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected post-expression token");
    case INDSTART:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[PIPE - 4] = NONE;
        allowed[ARGEXPREND - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "");
    case INDEND:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[PIPERES - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected token after ]");
    case CTRLSTART:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ACCESSOR - 4] = NONE;
        allowed[DELIMITER - 4] = NONE;
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLSTART - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected token after control");
    case CTRLEND:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[PIPERES - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected post-expression token");
    case PIPE:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBER);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ACCESSOR - 4] = NONE;
        allowed[DELIMITER - 4] = NONE;
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLSTART - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "expected expression after pipe");
    case PIPERES:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected token after pipe result");
    case SPREAD:
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[CTRLEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        allowed[END - 4] = NONE;
        return allowSingularTokens(c, allowed, "expected expression to spread");
    default:
        return Result<TokenType>("invalid token");
    }

    return Result<TokenType>("invalid token");
}

Result<std::vector<Token>> tokenize(std::string str)
{
    std::vector<Token> tokens;
    std::string accumulated = "";
    TokenType accType = NONE;
    bool prevWhitespace = false;
    for (int i = 0; i < str.size(); i++)
    {
        char c = str[i];
        Result<TokenType> result = getTokenType(c, accType, accumulated, prevWhitespace);
        if (!result.error.empty())
        {
            std::string msg = ("Column " + std::to_string(i + 1) + "\n" + result.error);
            msg = "Near \"" + str.substr(std::max(0, i - 3), 5) + "\"\n" + msg;
            return Result<std::vector<Token>>(msg);
        }
        if (result.value == NONE)
        {
            prevWhitespace = true;
            continue;
        }
        prevWhitespace = false;
        if (result.value != accType)
        {
            tokens.push_back(Token(accumulated, accType));
            accumulated = "";
        }
        accType = result.value;
        accumulated += c;
    }
    if (!accumulated.empty())
        tokens.push_back(Token(accumulated, accType));
    for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens[i].value << '\n';
    }
    return Result<std::vector<Token>>(tokens);
}

enum Context
{
    BASE,
    EXPR,
    DELIMITED,
    ARR,
    INDEXPR,
    IFEXPR,
    CTRLSEQ
};

void createAST(State &state, std::vector<Token> &tokens, int &index, Node *parent, Context context)
{
    // assume we are accumulating an expression
    Node *cur = nullptr;

    for (int i = index; i < tokens.size(); i++)
    {
        // handle operators: any operator
        // handle calls: function followed by (
        // handle expressions: (
        // handle indexing and arrays: [ or value followed by [
        switch (tokens[i].type)
        {
        case TEXT:
            // check for keyword
            if (state.isKeyword(tokens[i].value))
            {
                if (tokens[i].value == "if")
                {
                    createAST(state, tokens, ++i, parent, IFEXPR);
                }
            }
            else // variable or function
            {
                IdentifierNode *identifier = new IdentifierNode(tokens[i]);
                // if we are calling a function
                // as opposed to var or fn as value
                if (state.isFunction(tokens[i].value) && tokens.size() > i + 1 && tokens[i + 1].type == ARGEXPRSTART)
                {
                    identifier->makeCall();
                    cur = (Node *)identifier;
                    createAST(state, tokens, ++i, cur, DELIMITED);
                }
                // if we are indexing
                if (tokens.size() > i + 1 && tokens[i + 1].type == INDSTART)
                {
                    IndexNode *index = new IndexNode();
                    cur = index;
                    cur->addChild(identifier);
                    createAST(state, tokens, ++i, cur, INDEXPR);
                }
            }
        case NUMBER:
            // a regular number
            cur = new Node(Value(tokens[i]));
        case OPERATOR:
            OperationNode *op = new OperationNode(tokens[i]);
            // if binary op as opposed to unary
            if (cur)
                parent->addChild(op);
            cur = (Node *)op;
            createAST(state, tokens, ++i, cur, EXPR);
        case ACCESSOR:
            if (!cur)
                throw "unexpected accessor";
            if (tokens.size() > i + 1 && tokens[i + 1].type == TEXT && !state.isKeyword(tokens[i + 1].value))
            {
                AccessNode *accessor = new AccessNode(tokens[i + 1].value);
                accessor->addChild(cur);
                cur = accessor;
            }
        case DELIMITER:
            if (context == DELIMITED || context == ARR)
            {
                parent->addChild(cur);
                cur = nullptr;
            }
            else
                throw "unexpected comma";
        case ARGEXPRSTART:
            createAST(state, tokens, ++i, parent, EXPR);
        case ARGEXPREND:
            if (context == DELIMITED || context == EXPR)
            {
                if (cur)
                    parent->addChild(cur);
                return;
            }
            throw "unexpected end of expression";

        case INDSTART:
            // has to be an array
            cur = new Node(Value("arr"));
            createAST(state, tokens, ++i, cur, ARR);
        case INDEND:
            if (context == ARR || context == INDEXPR)
            {
                if (!cur)
                    throw "unexpected ]";
                parent->addChild(cur);
                return;
            }
            throw "unexpected end of array";

        case CTRLSTART:
            if (context == IFEXPR)
            {
                if (!cur)
                    throw "expected condition expression";
                BranchNode *branch = new BranchNode();
                // conditional expression
                branch->addChild(cur);
                cur = branch;
                createAST(state, tokens, ++i, cur, CTRLSEQ);
            }
            throw "unexpected {";
        case CTRLEND:

        case PIPE:
        }
    }
}

int main(int argc, char *argv[])
{
    std::string line = "";
    for (int i = 1; i < argc; i++)
    {
        line += argv[i];
        line += " ";
    }
    Result<std::vector<Token>> result = tokenize(line);
    if (!result.error.empty())
        std::cerr << result.error << '\n';
    return 0;
}

#include <iostream>
#include <stack>
#include <vector>
#include "PipeNode.h"
#include "SequenceNode.h"
#include "Token.h"
#include "OperationNode.h"
#include "IdentifierNode.h"
#include "IndexNode.h"
#include "AccessNode.h"
#include "BranchNode.h"
#include "SpreadNode.h"
#include "WrapperNode.h"

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
NUMBER      x x   x   x   x         x x
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
        if ((result.value != accType || SINGULARS.find(c) != std::string::npos) && accType != NONE && !accumulated.empty())
        {
            tokens.push_back(Token(accumulated, accType));
            accumulated = "";
        }
        accType = result.value;
        accumulated += c;
    }
    if (!accumulated.empty())
        tokens.push_back(Token(accumulated, accType));
    return Result<std::vector<Token>>(tokens);
}

enum Context
{
    BASE,
    EXPR,
    OPERATING,
    DELIMITED,
    ARR,
    INDEXPR,
    IFEXPR,
    CTRLSEQ,
};

const std::string CONTEXT[] = {"base", "expr", "operating", "delim", "arr", "indexpr", "ifexpr", "ctrlseq"};

const std::string HELP[] = {"none", "number", "operator", "text", "accessor", "delimiter", "argexpstart", "argexprend",
                            "indstart", "indend", "ctrlstart", "ctrlend", "pipe", "piperes", "spread", "end"};

const std::string NODETYPES[] = {"leafvalue", "identifier", "call", "operation", "assignment", "index", "sequence",
                                 "spread", "branch", "access", "pipe", "wrapper"};

std::string createAST(State &state, std::vector<Token> &tokens, int &index, Node *parent, Context context, bool piped)
{
    // assume we are accumulating an expression
    Node *cur = nullptr;

    for (; index < tokens.size(); index++)
    {

        Token token = tokens[index];
        std::cout << index << ' ' << token.value << ' ' << CONTEXT[(int)context] << '\n';
        //  handle operators: any operator
        //  handle calls: function followed by (
        //  handle expressions: (
        //  handle indexing and arrays: [ or value followed by [
        std::string error;
        switch (token.type)
        {
        case TEXT:
            // check for keyword
            if (state.isKeyword(token.value))
            {
                if (token.value == "if")
                {
                    error = createAST(state, tokens, ++index, parent, IFEXPR, piped);
                    if (!error.empty())
                        return error;
                }
            }
            else // variable or function
            {
                IdentifierNode *identifier = new IdentifierNode(token);
                cur = identifier;
                // if we are calling a function
                // as opposed to a var or fn as value
                if (tokens.size() > index + 1 && tokens[index + 1].type == ARGEXPRSTART)
                {
                    identifier->makeCall();
                    index++;
                    error = createAST(state, tokens, ++index, cur, DELIMITED, piped);
                    if (!error.empty())
                        return error;
                }
                // if we are indexing
                if (tokens.size() > index + 1 && tokens[index + 1].type == INDSTART)
                {
                    IndexNode *indexer = new IndexNode();
                    cur = indexer;
                    cur->addChild(indexer);
                    error = createAST(state, tokens, ++index, cur, INDEXPR, piped);
                    if (!error.empty())
                        return error;
                }
            }
            break;
        case NUMBER:
            // a regular number
            if (cur)
                return "unexpected number";
            else
                cur = new Node(Value(token));
            break;
        case OPERATOR:
            if (true)
            {
                OperationNode *op = new OperationNode(token);
                // if binary op as opposed to unary
                if (cur)
                    op->addChild(cur);
                cur = op;
                error = createAST(state, tokens, ++index, cur, OPERATING, piped);
                if (!error.empty())
                    return error;
            }
            break;
        case ACCESSOR:
            if (!cur)
                return "unexpected accessor";
            if (tokens.size() > index + 1 && tokens[index + 1].type == TEXT && !state.isKeyword(tokens[index + 1].value))
            {
                AccessNode *accessor = new AccessNode(tokens[index + 1].value);
                accessor->addChild(cur);
                cur = accessor;
            }
            break;
        case DELIMITER:
            if (context == OPERATING)
            {
                if (!cur)
                    return "unexpected ,";
                parent->addChild(cur);
                cur = nullptr;
                // backtrack to delimited context
                index--;
                return "";
            }
            else if (context == DELIMITED || context == ARR)
            {
                parent->addChild(cur);
                cur = nullptr;
            }
            else
                return "unexpected comma";
            break;
        case ARGEXPRSTART:
            if (!cur)
            {
                WrapperNode *wrapper = new WrapperNode();
                cur = wrapper;
                error = createAST(state, tokens, ++index, cur, EXPR, piped);
                if (!error.empty())
                    return error;
            }
            else
                return "unexpected (";
            break;
        case ARGEXPREND:
            // std::cout << "we hit ] " << tokens[index].value << " " << CONTEXT[context] << '\n';
            if (context == DELIMITED || context == EXPR || context == OPERATING)
            {
                if (cur)
                    parent->addChild(cur);
                // backtrack to end of operating context
                if (context == OPERATING)
                    index--;
                return "";
            }
            else
            {
                std::cout << CONTEXT[context] << '\n';
                return "unexpected end of expression";
            }
            break;

        case INDSTART:
            // has to be an array
            cur = new Node(Value("arr"));
            error = createAST(state, tokens, ++index, cur, ARR, piped);
            if (!error.empty())
                return error;
            break;
        case INDEND:
            // if an array or index expression ends
            if (context == OPERATING)
            {
                // jump out of expr
                if (cur)
                {
                    parent->addChild(cur);
                }
                // backtrack to end of operating context
                index--;
                return "";
            }
            else if (context == ARR || context == INDEXPR)
            {
                if (!cur && context == INDEXPR)
                    return "unexpected ]";
                if (cur)
                    parent->addChild(cur);
                return "";
            }
            else
                return "unexpected end of array";
            break;
        case CTRLSTART:
            if (context == IFEXPR)
            {
                if (!cur)
                    return "expected condition expression";
                BranchNode *branch = new BranchNode();
                // conditional expression
                branch->addChild(cur);
                cur = branch;
                error = createAST(state, tokens, ++index, cur, CTRLSEQ, piped);
                if (!error.empty())
                    return error;
            }
            return "unexpected {";
        case CTRLEND:
            if (context == CTRLSEQ)
            {
                if (cur)
                    parent->addChild(cur);
                return "";
            }
            return "unexpected }";
        case PIPE:
            if (true)
            {
                PipeNode *pipe = new PipeNode();
                // if binary op as opposed to unary
                if (!cur)
                    return "unexpected |";
                pipe->addChild(cur);
                cur = pipe;
                error = createAST(state, tokens, ++index, cur, OPERATING, true);
                if (!error.empty())
                    return error;
            }
            break;
        case PIPERES:
            if (piped)
            {
                IdentifierNode *identifier = new IdentifierNode();
                // if we are calling a function
                // as opposed to var or fn as value
                if (tokens.size() > index + 1 && tokens[index + 1].type == ARGEXPRSTART)
                {
                    identifier->makeCall();
                    cur = identifier;
                    error = createAST(state, tokens, ++index, cur, DELIMITED, piped);
                    if (!error.empty())
                        return error;
                }
                // if we are indexing
                else if (tokens.size() > index + 1 && tokens[index + 1].type == INDSTART)
                {
                    IndexNode *indexer = new IndexNode();
                    cur = indexer;
                    cur->addChild(identifier);
                    error = createAST(state, tokens, ++index, cur, INDEXPR, piped);
                    if (!error.empty())
                        return error;
                }
                else
                {
                    if (cur)
                        return "unexpected |";
                    cur = identifier;
                }
            }
            else
                return "unexpected &";
            break;
        case SPREAD:
            if (context == DELIMITED)
            {
                SpreadNode *spread = new SpreadNode();
                spread->addChild(cur);
                cur = spread;
                error = createAST(state, tokens, ++index, cur, EXPR, piped);
                if (!error.empty())
                    return error;
            }
            else
                return "unexpected ~";
        case END:
            if (!cur && context == OPERATING)
                return "unexpected ;";
            if (context == OPERATING)
            {
                if (cur)
                    parent->addChild(cur);
                index--;
                return "";
            }
            // proper new statement
            else if (context == BASE)
            {
                context = BASE;
                parent->addChild(cur);
                cur = nullptr;
                break;
            }
            return "unexpected end of statement";
        default:
            return "that wasn't supposed to happen";
        }
    }
    return "";
}

void printTree(const Node &node, int depth = 0)
{
    // Print the current node's value with indentation
    for (int i = 0; i < depth; ++i)
    {
        std::cout << "  "; // Indent based on the depth
    }
    std::cout << NODETYPES[(int)node.getType()] << ' ' << node.getChildren().size() << '\n';

    // Recursively print each child with increased depth
    for (const Node *child : node.getChildren())
    {
        printTree(*child, depth + 1);
    }
}

int main(int argc, char *argv[])
{
    std::string line = "";

    State state;
    std::cout << "Welcome to the Big C Lemur Engine!\n\n";
    while (true)
    {
        std::cout << ">> ";
        std::cin >> line;
        Result<std::vector<Token>> result = tokenize(line);

        if (!result.error.empty())
        {
            std::cerr << "Tokenization Error: " << result.error << '\n';
            continue;
        }
        std::cout << "token count: " << result.value.size() << '\n';
        SequenceNode *program = new SequenceNode();
        int index = 0;
        std::string error = createAST(state, result.value, index, program, BASE, false);
        if (!error.empty())
        {
            std::cout << "Interpretation Error: " << error << '\n';
            printTree(*program);
            continue;
        }
        std::cout << "\n";
        printTree(*program);
    }
    return 0;
}

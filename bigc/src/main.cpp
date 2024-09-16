#include <iostream>
#include <fstream>
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
#include "LoopNode.h"
#include "FunctionNode.h"
#include "Array.h"
#include "TypeNode.h"

const std::string OPERATORS = "+-*!/=><@";
const std::string DIGITS = "0123456789";
const std::string WORDS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_:";
const std::string SINGULARS = ".,()[]{}|&~;";
const std::string KEYWORDS[] = {"if"};

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
TEXT                                x x
NUMBERSTR     x x   x   x   x         x x
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
    case END:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBERSTR);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[ACCESSOR - 4] = NONE;
        allowed[DELIMITER - 4] = NONE;
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
        allowed[PIPE - 4] = NONE;
        allowed[PIPERES - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected end of statement");

    case NONE:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBERSTR);
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

    case NUMBERSTR:
        if (c == '.' && hasDot)
            return Result<TokenType>("why dot");
        if (DIGITS.find(c) != std::string::npos || (!hasDot && c == '.'))
            return Result<TokenType>(NUMBERSTR);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[ARGEXPRSTART - 4] = NONE;
        allowed[INDSTART - 4] = NONE;
        allowed[PIPERES - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "invalid token after number or string");
    case TEXT:
        if (prevWhitespace && DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBERSTR);
        if (WORDS.find(c) != std::string::npos || DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[PIPERES - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "invalid token after word");
    case OPERATOR:
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBERSTR);
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[ARGEXPREND - 4] = NONE;
        allowed[INDEND - 4] = NONE;
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
            return Result<TokenType>(NUMBERSTR);
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
            return Result<TokenType>(NUMBERSTR);
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
            return Result<TokenType>(NUMBERSTR);
        */
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        allowed[PIPERES - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected post-expression token");
    case INDSTART:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBERSTR);
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
            return Result<TokenType>(NUMBERSTR);
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
        if (WORDS.find(c) != std::string::npos)
            return Result<TokenType>(TEXT);
        allowed[PIPERES - 4] = NONE;
        allowed[SPREAD - 4] = NONE;
        return allowSingularTokens(c, allowed, "unexpected post-expression token");
    case PIPE:
        if (OPERATORS.find(c) != std::string::npos)
            return Result<TokenType>(OPERATOR);
        if (DIGITS.find(c) != std::string::npos)
            return Result<TokenType>(NUMBERSTR);
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
    bool inString = false;
    for (int i = 0; i < str.size(); i++)
    {
        char c = str[i];
        // handle string
        if (c == '"' && !inString)
        {
            if (!accumulated.empty())
                tokens.push_back(Token(accumulated, accType));
            accumulated = "";
            inString = true;
            continue;
        }
        else if (c == '"' && inString && str[i - 1] != '\\')
        {
            inString = false;
            // differenatiate between strs and numbers
            tokens.push_back(Token('s' + accumulated, NUMBERSTR));
            accumulated = "";
            continue;
        }
        else if (inString)
        {
            if (c != '\\')
                accumulated += c;
            continue;
        }
        Result<TokenType> result = getTokenType(c, accType, accumulated, prevWhitespace);
        if (!result.ok())
        {
            std::string msg = ("Column " + std::to_string(i + 1) + "\n" + result.getError());
            msg = "Near \"" + str.substr(std::max(0, i - 3), 5) + "\"\n" + msg;
            return Result<std::vector<Token>>(msg);
        }
        if (result.getValue() == NONE)
        {
            prevWhitespace = true;
            if (!accumulated.empty())
            {
                if (accType == NUMBERSTR)
                {
                    accumulated = 'n' + accumulated;
                }
                tokens.push_back(Token(accumulated, accType));
                accumulated = "";
            }
            continue;
        }
        prevWhitespace = false;
        if ((result.getValue() != accType || SINGULARS.find(c) != std::string::npos) && accType != NONE && !accumulated.empty())
        {

            if (accType == NUMBERSTR)
                accumulated = 'n' + accumulated;
            tokens.push_back(Token(accumulated, accType));
            accumulated = "";
        }
        accType = result.getValue();
        accumulated += c;
    }
    if (!accumulated.empty())
    {
        // differentiate between numbers and strs
        if (accType == NUMBERSTR)
            accumulated = 'n' + accumulated;
        tokens.push_back(Token(accumulated, accType));
    }
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
    IFSEQ,
    ELSEEXPR,
    SEQ,
    LOOPEXPR,
    FUNDEFARGS
};

const std::string CONTEXT[] = {"base", "expr", "operating", "delim", "arr", "indexpr", "ifexpr", "ifseq", "elseexpr", "seq", "loopexpr", "fundefargs"};

const std::string HELP[] = {"none", "numberstr", "operator", "text", "accessor", "delimiter", "argexpstart", "argexprend",
                            "indstart", "indend", "ctrlstart", "ctrlend", "pipe", "piperes", "spread", "end"};

const std::string NODETYPES[] = {"leafvalue", "identifier", "call", "operation", "assignment", "index", "sequence",
                                 "spread", "branch", "access", "pipe", "wrapper", "unbranch", "loop", "return"};

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
                    if (cur)
                        return "unexpected if";
                    // look for condition
                    cur = new BranchNode();
                    error = createAST(state, tokens, ++index, cur, IFEXPR, piped);
                    if (!error.empty())
                        return error;
                }
                else if (token.value == "while")
                {
                    if (cur)
                        return "unexpected while";
                    // look for condition
                    cur = new LoopNode();
                    error = createAST(state, tokens, ++index, cur, LOOPEXPR, piped);
                    if (!error.empty())
                        return error;
                }
                else if (token.value == "funion")
                {
                    if (cur)
                        return "unexpected function definition";
                    cur = new FunctionNode();
                    error = createAST(state, tokens, ++index, cur, FUNDEFARGS, piped);
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
        case NUMBERSTR:
            // a regular number or string
            if (cur)
                return "unexpected number or string";
            else
                cur = new Node(Value(token));
            break;
        case OPERATOR:
            if (context != FUNDEFARGS)
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
            else
            {
                // the operator must be a cast
                if (token.value != "@")
                    return "only the cast operator is allowed in function definitions";
                if (!cur)
                    return "unexpected @";
                if (tokens.size() > index + 1 && tokens[index + 1].type == TEXT)
                {
                    TypeNode *type = new TypeNode(tokens[index + 1].value);
                    type->addChild(cur);
                    cur = nullptr;
                }
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
            else if (context == DELIMITED || context == ARR || context == FUNDEFARGS)
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
            cur = new Node(Value(new Array<Value>()));
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
            // start of a function
            if (context == FUNDEFARGS)
            {
                if (cur)
                    parent->addChild(cur);
                SequenceNode *funcseq = new SequenceNode();
                parent->addChild(funcseq);
                createAST(state, tokens, ++index, funcseq, SEQ, piped);
                if (!error.empty())
                    return error;
                return "";
            }
            else if (context == LOOPEXPR)
            {
                if (cur)
                    parent->addChild(cur);
                SequenceNode *loop = new SequenceNode();
                parent->addChild(loop);
                // the sequence for the loop
                error = createAST(state, tokens, ++index, loop, SEQ, piped);
                if (!error.empty())
                    return error;
                return "";
            }
            else if (context == IFEXPR)
            {
                if (!cur)
                    return "expected condition expression";
                // conditional expression
                SequenceNode *ifseq = new SequenceNode();
                parent->addChild(ifseq);
                error = createAST(state, tokens, ++index, ifseq, IFSEQ, piped);
                if (!error.empty())
                    return error;
                // after the sequence ends we will have hit the ctrlend
                // there is no kickback so we are on the }
                // check for else and if so, add an unbramch
                if (tokens.size() > index + 2 && tokens[index + 1].type == TEXT)
                {
                    if (tokens[index + 1].value == "else" && tokens[index + 2].type == CTRLSTART)
                    {
                        std::cout << "elseasting\n";
                        SequenceNode *elseseq = new SequenceNode();
                        // add the else sequence after the if sequence on the branch
                        parent->addChild(elseseq);
                        index += 2;
                        error = createAST(state, tokens, ++index, elseseq, IFSEQ, piped);
                        if (!error.empty())
                            return error;
                    }
                }
                return "";
            }
            else if (context == OPERATING)
            {
                if (cur)
                    parent->addChild(cur);
                --index;
                return "";
            }
            // general sequence
            else if (!cur)
            {
                cur = new SequenceNode();
                error = createAST(state, tokens, ++index, cur, SEQ, piped);
                if (!error.empty())
                    return error;
            }
            else
                return "unexpected {";
            break;
        case CTRLEND:
            if (context == IFSEQ)
            {
                if (cur)
                    parent->addChild(cur);
                return "";
            }
            // end of general sequence
            else if (context == SEQ)
            {
                if (cur)
                    parent->addChild(cur);
                return "";
            }
            else
                return "unexpected }";
            break;
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
            break;
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
            else if (context == SEQ || context == IFSEQ)
            {
                if (cur)
                    parent->addChild(cur);
                cur = nullptr;
            }
            // proper new statement
            else if (context == BASE)
            {
                context = BASE;
                parent->addChild(cur);
                cur = nullptr;
                break;
            }
            else
                return "unexpected end of statement";
            break;
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

void printValue(Value value)
{
    std::cout << "type: " << value.getType() << ' ' << '\n';
    Wildcard val = value.getValue();
    if (value.getType() == "nil" || (std::get_if<bool *>(&val) && *(std::get_if<bool *>(&val)) == nullptr))
        std::cout << "nil\n";
    if (bool **x = std::get_if<bool *>(&val))
        std::cout << ((**x) ? "true" : "false") << '\n';
    else if (int **x = std::get_if<int *>(&val))
        std::cout << **x << '\n';
    else if (long **x = std::get_if<long *>(&val))
        std::cout << **x << '\n';
    else if (float **x = std::get_if<float *>(&val))
        std::cout << **x << '\n';
    else if (double **x = std::get_if<double *>(&val))
        std::cout << **x << '\n';
    else if (char **x = std::get_if<char *>(&val))
        std::cout << **x << '\n';
    else if (std::string **x = std::get_if<std::string *>(&val))
        std::cout << **x << '\n';
    else if (Iterable<Value> **x = std::get_if<Iterable<Value> *>(&val))
        std::cout << "arr" << '\n';
    else
        std::cout << "no print implemented" << '\n';
}

int main(int argc, char *argv[])
{
    State state;

    if (argc == 2)
    {
        std::ifstream file;
        try
        {
            file = std::ifstream(argv[1]);
        }
        catch (...)
        {
            std::cout << "failed to find file";
            return 0;
        }
        std::string programText;
        std::string text;
        while (file >> text)
        {
            programText += ' ' + text + ' ';
        }
        Result<std::vector<Token>> result = tokenize(programText);

        if (!result.ok())
        {
            std::cout << "Tokenization Error: " << result.getError() << '\n';
            return 0;
        }
        std::cout << "token count: " << result.getValue().size() << '\n';
        SequenceNode *program = new SequenceNode();
        int index = 0;
        auto tokens = result.getValue();
        std::string error = createAST(state, tokens, index, program, BASE, false);
        if (!error.empty())
        {
            std::cout << "Interpretation Error: " << error << '\n';
            printTree(*program);
            return 0;
        }
        std::cout << "\n";
        printTree(*program);
        std::cout << "\n";
        error = program->resolve(state);
        if (!error.empty())
        {
            std::cout << "Runtime Error:\n"
                      << error << '\n';
            return 0;
        }
        Value value = program->getChildren().back()->getValue(state);
        Wildcard valueValue = value.getValue();
        printValue(value);
        return 0;
    }
    std::string line = "";

    std::cout << "Welcome to the Big C Lemur Engine!\n\n";
    while (true)
    {
        std::cout << ">> ";
        std::cin >> line;
        if (line == "exit")
            break;
        Result<std::vector<Token>> result = tokenize(line);

        if (!result.ok())
        {
            std::cout << "Tokenization Error: " << result.getError() << '\n';
            continue;
        }
        std::cout << "token count: " << result.getValue().size() << '\n';
        SequenceNode *program = new SequenceNode();
        int index = 0;
        auto tokens = result.getValue();
        std::string error = createAST(state, tokens, index, program, BASE, false);
        if (!error.empty())
        {
            std::cout << "Interpretation Error: " << error << '\n';
            printTree(*program);
            continue;
        }
        std::cout << "\n";
        printTree(*program);
        std::cout << "\n";
        program->resolve(state);
        Value value = program->getChildren().back()->getValue(state);
        Wildcard valueValue = value.getValue();
        std::cout << "type: " << value.getType() << ' ' << '\n';
        if (int **x = std::get_if<int *>(&valueValue))
            std::cout << **x << '\n';
    }
    return 0;
}

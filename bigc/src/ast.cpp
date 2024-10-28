#include "ast.h"
#include <fstream>

namespace ast
{

    const std::string CONTEXT[] = {"base", "expr", "operating", "delim", "arr", "indexpr", "ifexpr", "ifseq", "elseexpr", "seq",
                                   "loopexpr", "fundefargs", "signal", "classargs", "classseq", "attdecl", "interfargs"};

    const std::string HELP[] = {"none", "numberstr", "operator", "text", "accessor", "delimiter", "argexpstart", "argexprend",
                                "indstart", "indend", "ctrlstart", "ctrlend", "pipe", "piperes", "spread", "end"};

    // drops you right before the next non-softend token
    void skipSoftends(std::vector<Token> &tokens, int &index)
    {
        while (tokens[index].type == SOFTEND)
        {
            index++;
            if (tokens[index].type != SOFTEND || index == tokens.size())
            {
                index--;
                break;
            }
        }
    }

    std::string createAST(State &state, std::vector<Token> &tokens, int &index, Node *parent, Context context, bool piped)
    {
        // assume we are accumulating an expression
        Node *cur = nullptr;

        for (; index < tokens.size(); index++)
        {
            skipSoftends(tokens, index);
            Token token = tokens[index];
            std::cout << index << ' ' << (token.value == "\n" ? "\\n" : token.value) << ' ' << CONTEXT[(int)context] << '\n';
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
                    else if (token.value == "collect")
                    {
                        if (cur)
                            return "unexpected collect";
                        // look for condition
                        cur = new CollectNode();
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
                    else if (token.value == "interface")
                    {
                        if (cur)
                            return "unexpected interface definition";
                        cur = new InterfaceNode();
                        // if we have an interface name we expect a ( and go until )
                        skipSoftends(tokens, index);
                        if (tokens.size() > index + 1 && tokens[index + 1].type == ARGEXPRSTART)
                        {
                            ++index;
                            error = createAST(state, tokens, ++index, cur, INTERFARGS, piped);
                            if (!error.empty())
                                return error;
                        }
                        else
                            return "expected ( for interface methods";
                    }
                    else if (token.value == "class")
                    {
                        // this works similarly to functions
                        if (cur)
                            return "unexpected class definition";
                        cur = new ClassNode();
                        error = createAST(state, tokens, ++index, cur, CLASSARGS, piped);
                        if (!error.empty())
                            return error;
                    }
                    else if (token.value == "return")
                    {
                        // continue with the same context
                        cur = new SignalNode(RETURN);
                        // this functions a lot like operating but allows having no children
                        error = createAST(state, tokens, ++index, cur, SIGNAL, piped);
                        if (!error.empty())
                            return error;
                    }
                    else if (token.value == "break")
                    {
                        cur = new SignalNode(BREAK);
                        error = createAST(state, tokens, ++index, cur, SIGNAL, piped);
                        if (!error.empty())
                            return error;
                    }
                    else if (token.value == "continue")
                    {
                        cur = new SignalNode(CONTINUE);
                        error = createAST(state, tokens, ++index, cur, SIGNAL, piped);
                        if (!error.empty())
                            return error;
                    }
                    else if (token.value == "group")
                    {
                        if (cur)
                            return "unexpected group";
                        skipSoftends(tokens, index);
                        if (tokens.size() > index + 1 && tokens[index + 1].type == TEXT)
                        {
                            ++index;
                            cur = new GroupNode(tokens[index].value);
                        }
                        else
                            return "expected identifier for group";
                    }
                    else if (token.value == "register")
                    {
                        if (cur)
                            return "unexpected register";
                        skipSoftends(tokens, index);
                        if (tokens.size() > index + 1 && tokens[index + 1].type == TEXT)
                        {
                            ++index;
                            cur = new RegisterNode(tokens[index].value);
                            // register expects one value
                            error = createAST(state, tokens, ++index, cur, SIGNAL, piped);
                            if (!error.empty())
                                return error;
                        }
                        else
                            return "expected identifier for register";
                    }
                    else if (token.value == "iterate")
                    {
                        if (cur)
                            return "unexpected iterate";
                        skipSoftends(tokens, index);
                        if (tokens.size() > index + 1 && tokens[index + 1].type == TEXT)
                        {
                            ++index;
                            cur = new IterateNode(tokens[index].value);
                            // this functions similarly to while so we can use LOOPEXPR
                            error = createAST(state, tokens, ++index, cur, LOOPEXPR, piped);
                            if (!error.empty())
                                return error;
                        }
                        else
                            return "expected identifier for iterate";
                    }
                    else if (token.value == "public" || token.value == "private" || token.value == "protected")
                    {
                        // sets the access
                        VisibilityNode *accessSpecifier = new VisibilityNode(token.value);
                        if (cur)
                            return "unexpected access specifier";
                        // we either have a method, identifier, or static
                        cur = accessSpecifier;
                        // check 2 for the possibilities of it being static or a method
                        skipSoftends(tokens, index);
                        if (tokens.size() > index + 2 && tokens[index + 1].type == TEXT)
                        {
                            ++index;
                            // static attribute
                            if (tokens[index].value == "shared")
                            {
                                accessSpecifier->makeStatic();
                                ++index;
                                if (tokens[index].type != TEXT)
                                    return "expected identifier for shared attribute";
                                accessSpecifier->setVariable(tokens[index].value);
                                error = createAST(state, tokens, ++index, cur, ATTDECL, piped);
                                if (!error.empty())
                                    return error;
                            }
                            // non-static method
                            else if (tokens[index].value == "method")
                            {
                                accessSpecifier->makeMethod();
                                ++index;
                                skipSoftends(tokens, index);
                                if (tokens[index].type != TEXT)
                                    return "expected identifier for method";
                                accessSpecifier->setVariable(tokens[index].value);
                                FunctionNode *fun = new FunctionNode();
                                cur->addChild(fun);
                                error = createAST(state, tokens, ++index, fun, FUNDEFARGS, piped);
                                if (!error.empty())
                                    return error;
                            }
                            // static method
                            else if (tokens[index].value == "utility")
                            {
                                accessSpecifier->makeMethod();
                                accessSpecifier->makeStatic();
                                ++index;
                                if (tokens[index].type != TEXT)
                                    return "expected identifier for utility";
                                FunctionNode *fun = new FunctionNode();
                                accessSpecifier->setVariable(tokens[index].value);
                                cur->addChild(fun);
                                error = createAST(state, tokens, ++index, fun, FUNDEFARGS, piped);
                                if (!error.empty())
                                    return error;
                            }
                            // non-static attribute
                            else
                            {
                                // this functions like fundefargs
                                accessSpecifier->setVariable(tokens[index].value);
                                error = createAST(state, tokens, ++index, cur, ATTDECL, piped);
                                if (!error.empty())
                                    return error;
                            }
                        }
                        else
                            return "expected 'method', 'static', or an identifier";
                    }
                }
                else // variable or function
                {
                    IdentifierNode *identifier = new IdentifierNode(token);
                    cur = identifier;
                    /*
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
                    */
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
                if (context == FUNDEFARGS || context == INTERFARGS)
                {
                    // the operator must be a cast
                    if (token.value != "@")
                        return "only the cast operator is allowed in function definitions";
                    if (!cur)
                        return "unexpected @";
                    skipSoftends(tokens, index);
                    if (tokens.size() > index + 1 && tokens[index + 1].type == TEXT)
                    {
                        TypeNode *type = new TypeNode(tokens[index + 1].value);
                        type->addChild(cur);
                        cur = nullptr;
                    }
                }
                // we are declaring an attribute
                else if (context == ATTDECL)
                {
                    if (token.value == "@")
                    {
                        // cur should be an identifier
                        skipSoftends(tokens, index);
                        if (tokens.size() > index + 1 && tokens[index + 1].type == TEXT)
                        {
                            ++index;
                            if (VisibilityNode *visibility = dynamic_cast<VisibilityNode *>(parent))
                                visibility->setType(tokens[index].value);
                            else
                                return "expected visibility node";
                        }
                        else
                            return "expected type after @";
                    }
                    else if (token.value == "=")
                    {
                        // check the next tokens as just a regular value
                        // the SIGNAL context will handle this already
                        // this is the default value

                        error = createAST(state, tokens, ++index, parent, SIGNAL, piped);
                        if (!error.empty())
                            return error;
                    }
                    else
                        return "unexpected operator in attribute declaration";
                }
                else
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
                if (cur)
                {
                    // while the next token is text and not a keyword
                    while (tokens.size() > index + 1 && tokens[index + 1].type == TEXT && !state.isKeyword(tokens[index + 1].value))
                    {
                        ++index;
                        AccessNode *accessor = new AccessNode(tokens[index].value);
                        accessor->addChild(cur);
                        cur = accessor;
                        skipSoftends(tokens, index);
                        if (tokens.size() > index + 1 && tokens[index + 1].type == ACCESSOR)
                            ++index;
                        else
                            break;
                    }
                }
                else
                    return "unexpected accessor";
                break;
            case DELIMITER:
                if (context == OPERATING || context == SIGNAL)
                {
                    if (!cur)
                        return "unexpected ,";
                    parent->addChild(cur);
                    cur = nullptr;
                    // backtrack to delimited context
                    index--;
                    return "";
                }
                else if (context == DELIMITED || context == ARR || context == FUNDEFARGS || context == CLASSARGS || context == INTERFARGS)
                {
                    parent->addChild(cur);
                    cur = nullptr;
                }
                else
                    return "unexpected comma";
                break;
            case ARGEXPRSTART:
                if (cur)
                {
                    // This is a function call
                    CallNode *callNode = new CallNode(cur);
                    cur = callNode;
                    error = createAST(state, tokens, ++index, cur, DELIMITED, piped);
                    if (!error.empty())
                        return error;
                }
                else
                {
                    // This is a regular parenthesized expression
                    WrapperNode *wrapper = new WrapperNode();
                    cur = wrapper;
                    error = createAST(state, tokens, ++index, cur, EXPR, piped);
                    if (!error.empty())
                        return error;
                }
                break;

            case ARGEXPREND:
                if (context == DELIMITED || context == EXPR || context == OPERATING || context == SIGNAL || context == INTERFARGS)
                {
                    if (cur)
                        parent->addChild(cur);
                    // backtrack to end of operating context
                    if (context == OPERATING || context == SIGNAL)
                        index--;
                    return "";
                }
                else
                {
                    return "unexpected end of expression";
                }
                break;

            case INDSTART:
                // if cur has a value we are indexing
                if (cur)
                {
                    IndexNode *indexer = new IndexNode();
                    indexer->addChild(cur);
                    cur = indexer;
                    error = createAST(state, tokens, ++index, cur, INDEXPR, piped);
                    if (!error.empty())
                        return error;
                }
                else
                {
                    cur = new Node(Value(new Array<Value>()));
                    error = createAST(state, tokens, ++index, cur, ARR, piped);
                    if (!error.empty())
                        return error;
                }
                break;
            case INDEND:
                // if an array or index expression ends
                if (context == OPERATING || context == SIGNAL)
                {
                    // jump out of expr
                    if (cur)
                        parent->addChild(cur);
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
                    // when returning we are on the indend
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
                else if (context == CLASSARGS)
                {
                    if (cur)
                        parent->addChild(cur);
                    SequenceNode *classseq = new SequenceNode();
                    parent->addChild(classseq);
                    createAST(state, tokens, ++index, classseq, CLASSSEQ, piped);
                    if (!error.empty())
                        return error;
                    return "";
                }
                else if (context == LOOPEXPR)
                {
                    // loop condition
                    if (!cur)
                        return "expected loop condition";
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
                    // the parent is the branch node
                    if (!cur)
                        return "expected condition expression";
                    // conditional expression
                    parent->addChild(cur);
                    // create the sequence
                    SequenceNode *ifseq = new SequenceNode();
                    parent->addChild(ifseq);
                    // populate the sequence
                    error = createAST(state, tokens, ++index, ifseq, IFSEQ, piped);
                    if (!error.empty())
                        return error;
                    // after the sequence ends we will have hit the ctrlend
                    // there is no kickback so we are on the }
                    // check for else and if so, add a sequence for that
                    skipSoftends(tokens, index);
                    if (tokens.size() > index + 2 && tokens[index + 1].type == TEXT)
                    {
                        if (tokens[index + 1].value == "else")
                        {
                            skipSoftends(tokens, index);
                            if (tokens[index + 1].type == CTRLSTART)
                            {
                                SequenceNode *elseseq = new SequenceNode();
                                ++index;
                                // add the else sequence after the if sequence on the branch
                                parent->addChild(elseseq);
                                error = createAST(state, tokens, ++index, elseseq, IFSEQ, piped);
                                if (!error.empty())
                                    return error;
                            }
                        }
                    }
                    // return to where we initialized the ifexpr
                    return "";
                }
                else if (context == OPERATING || context == SIGNAL)
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
                else if (context == CLASSSEQ)
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
                    cur = identifier;
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
            case SOFTEND:
                skipSoftends(tokens, index);
                // if we're operating with cur, assume this will be an end
                if (cur && (context == OPERATING || context == SIGNAL || context == ATTDECL))
                {
                    if (cur)
                        parent->addChild(cur);
                    index--;
                    return "";
                }
                // if we aren't in a sequence-like context ignore this token
                // otherwise we bleed into end
                if (context != BASE && context != SEQ && context != IFSEQ && context != CLASSSEQ)
                    break;
                if (!cur)
                    break;
            case END:
                if (!cur && context == OPERATING)
                    return "unexpected ;";
                if (context == OPERATING || context == SIGNAL || context == ATTDECL)
                {
                    if (cur)
                        parent->addChild(cur);
                    index--;
                    return "";
                }
                // add cur to the sequence and potentially add more
                else if (context == SEQ || context == IFSEQ || context == CLASSSEQ)
                {
                    if (cur)
                        parent->addChild(cur);
                    cur = nullptr;
                }
                // proper new statement
                else if (context == BASE)
                {
                    if (cur)
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

    void printTree(const Node &node, int depth)
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

    Result<State> evaluate(std::string filename)
    {
        State state;
        std::ifstream file(filename);
        std::string programText;
        std::string line;
        while (std::getline(file, line))
        {
            programText += line + '\n';
        }
        Result<std::vector<Token>> result = tokenizer::tokenize(programText);
        if (!result.ok())
            return state;
        SequenceNode *program = new SequenceNode();
        int index = 0;
        std::vector<Token> tokens = result.getValue();
        std::string error = createAST(state, tokens, index, program, BASE, false);
        if (!error.empty())
            return Result<State>(error);
        return Result<State>(state);
    }
}
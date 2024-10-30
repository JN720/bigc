#pragma once

#include "Node.h"
#include "tokenizer.h"
#include <vector>
#include "PipeNode.h"
#include "SequenceNode.h"
#include "OperationNode.h"
#include "IdentifierNode.h"
#include "IndexNode.h"
#include "AccessNode.h"
#include "BranchNode.h"
#include "SpreadNode.h"
#include "WrapperNode.h"
#include "LoopNode.h"
#include "Array.h"
#include "TypeNode.h"
#include "FunctionNode.h"
#include "SignalNode.h"
#include "CollectNode.h"
#include "ClassNode.h"
#include "InterfaceNode.h"
#include "VisibilityNode.h"
#include "RegisterNode.h"
#include "CallNode.h"
#include "GroupNode.h"
#include "IterateNode.h"
#include "TryNode.h"
namespace ast
{

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
        FUNDEFARGS,
        SIGNAL,
        CLASSARGS,
        CLASSSEQ,
        ATTDECL,
        INTERFARGS
    };

    void skipSoftends(std::vector<Token> &tokens, int &index);
    std::string createAST(State &state, std::vector<Token> &tokens, int &index, Node *parent, Context context, bool piped);
    void printTree(const Node &node, int depth);
    Result<State> evaluate(std::string filename);
}

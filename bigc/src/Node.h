#pragma once
#include <vector>
#include "Array.h"
#include "State.h"
#include "Control.h"

enum NodeType
{
    N_LEAFVALUE,
    N_IDENTIFIER,
    N_CALL,
    N_OPERATION,
    N_ASSIGNMENT,
    N_INDEX,
    N_SEQUENCE,
    N_SPREAD,
    N_BRANCH,
    N_ACCESS,
    N_PIPE,
    N_WRAPPER,
    N_UNBRANCH,
    N_LOOP,
    N_SIGNAL,
    N_CLASS,
    N_VISIBILITY,
    N_INTERFACE,
    N_LIBRARY,
    N_REGISTER,
    N_GROUP,
    N_ITERATE,
    N_TYPE,
    N_TRY
};

const std::string NODETYPES[] = {"leafvalue", "identifier", "call", "operation", "assignment", "index", "sequence",
                                 "spread", "branch", "access", "pipe", "wrapper", "unbranch", "loop", "signal",
                                 "class", "visibility", "interface", "library", "register", "group", "iterate",
                                 "type", "try"};

class Node : public Allocated
{
public:
    Node();
    Node(Value value);
    Node(std::vector<Node *> &children);
    virtual Value getValue(const State &state);
    virtual Control resolve(State &state);
    void addChild(Node *child);
    NodeType getType() const;
    std::vector<Node *> getChildren() const;
    bool hasChildren() const;
    virtual Node *copy();
    virtual void destroy(State *state) override;

protected:
    std::vector<Node *> children;
    Value value;
    NodeType type;
};

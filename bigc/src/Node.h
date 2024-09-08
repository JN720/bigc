#pragma once
#include <vector>
#include "Array.h"
#include "State.h"

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
    N_UNBRANCH
};

class Node
{
public:
    Node();
    Node(Value value);
    Node(std::vector<Node *> &children);
    virtual Value getValue(const State &state);
    virtual std::string resolve(State &state);
    void addChild(Node *child);
    NodeType getType() const;
    std::vector<Node *> getChildren() const;

protected:
    std::vector<Node *> children;
    Value value;
    NodeType type;
};

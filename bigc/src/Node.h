#pragma once
#include <vector>
#include "Value.h"
#include "State.h"
enum NodeType
{
    LEAFVALUE,
    IDENTIFIER,
    CALL,
    OPERATION,
    ASSIGNMENT,
    INDEX
};

class Node
{
public:
    Node();
    Node(Value value);
    Node(std::vector<Node *> &children);
    virtual Value getValue(const State &state);
    void addChild(Node *child);

protected:
    std::vector<Node *> children;
    Value value;
    NodeType type;
};

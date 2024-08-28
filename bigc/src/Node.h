#pragma once
#include<vector>
#include "Value.h"

enum NodeType {
    LEAFVALUE,
    CALL,
    OPERATION,
    ASSIGNMENT,
    INDEX
};

class Node {
public:
    Node(Value &value);
    Node(std::vector<Node> children);
    void *getValue();
protected:
    std::vector<Node> children;
    Value value;
    NodeType type;

};

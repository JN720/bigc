#pragma once
#include "Node.h"
#include "Value.h"

Node::Node(Value &value) {
    this->value = value;
}

Node::Node(std::vector<Node> children) {
    this->children = children;
}

void Node::getValue() {

}

void Node::addChild(const Node) {

}

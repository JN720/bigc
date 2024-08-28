#include "Node.h"

class OperationNode : public Node {
public:
    void *getValue();
    OperationNode(char operation);
protected:
    char op;
};

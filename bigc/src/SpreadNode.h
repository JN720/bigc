#include "Node.h"


class SpreadNode : public Node {
public:
    SpreadNode();
    Value getValue(const State &state) override;
};

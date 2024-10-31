#ifndef SOCKET_FUNDAMENTAL_CLASS_H
#define SOCKET_FUNDAMENTAL_CLASS_H

#include "../../FundamentalClassDefinition.h"
#include "SocketFundamentalObject.h"

class SocketFundamentalClass : public FundamentalClassDefinition
{
public:
    SocketFundamentalClass();
    Result<Value> construct(State *state, std::vector<Node *> &args) override;
};

#endif // SOCKET_FUNDAMENTAL_CLASS_H

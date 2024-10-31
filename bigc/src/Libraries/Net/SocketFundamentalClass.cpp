#include "SocketFundamentalClass.h"
#include "SocketFundamentalObject.h"
#include "SocketFundamentalMethodNode.h"

SocketFundamentalClass::SocketFundamentalClass()
{
    // Add methods for socket operations
    addMethod("connect", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                         {
                                                            if (args.size() != 2)
                                                                return Result<Value>("socket connection requires 2 arguments: address and port");
                                                            Wildcard val = args[0]->getValue(state).getValue();
                                                            std::string **ipAddress = std::get_if<std::string *>(&val);
                                                            if (!ipAddress)
                                                                return Result<Value>("ip address must be a string");
                                                            if ((*ipAddress)->empty())
                                                                return Result<Value>("ip address cannot be empty");
                                                            val = args[1]->getValue(state).getValue();
                                                            int *port = std::get_if<int>(&val);
                                                            if (!port)
                                                                return Result<Value>("port must be an integer");
                                                            return socketObj->connect(**ipAddress, *port); }),
              false, PUBLIC);

    addMethod("send", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                      { return Result<Value>("not implemented"); }),
              false, PUBLIC);

    addMethod("receive", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                         {
                                                             // Implement receive logic
return Result<Value>("not implemented"); }),
              false, PUBLIC);

    addMethod("close", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                       {
                                                           // Implement close logic
return Result<Value>("not implemented"); }),
              false, PUBLIC);
}

Result<Value> SocketFundamentalClass::construct(State *state, std::vector<Node *> &args)
{
    SocketFundamentalObject *socketObj = new SocketFundamentalObject();
    if (!socketObj->isOpen())
        return Result<Value>("Socket creation failed");
    return Result<Value>(Value(socketObj));
}
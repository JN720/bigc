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

    addMethod("sendStr", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                         {
                                                            if (args.size() != 1)
                                                                return Result<Value>("socket sendStr requires 1 argument");
                                                            Wildcard val = args[0]->getValue(state).getValue();
                                                            std::string **str = std::get_if<std::string *>(&val);
                                                            if (!str)
                                                                return Result<Value>("data must be a string");
                                                            if ((*str)->empty())
                                                                return Result<Value>("data cannot be empty");
                                                            return socketObj->send(**str); }),
              false, PUBLIC);

    addMethod("receive", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                         {
                                                            if (args.size())
                                                                return Result<Value>("socket receive does not take any arguments");
                                                            return socketObj->receive(); }),
              false, PUBLIC);

    addMethod("close", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                       {
                                                            if (args.size())
                                                                return Result<Value>("socket close does not take any arguments");
                                                            socketObj->close(); 
                                                            return Result<Value>(Value(true)); }),
              false, PUBLIC);

    addMethod("listen", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                        {
                                                            if (args.size() != 1)
                                                                return Result<Value>("socket listen requires 1 argument: backlog");
                                                            Wildcard val = args[0]->getValue(state).getValue();
                                                            int *backlog = std::get_if<int>(&val);
                                                            if (!backlog)
                                                                return Result<Value>("backlog must be an integer");
                                                            return socketObj->listen(*backlog); }),
              false, PUBLIC);

    addMethod("accept", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                        {
                                                            if (args.size())
                                                                return Result<Value>("socket accept does not take any arguments");
                                                            return socketObj->accept(); }),
              false, PUBLIC);
}

Result<Value> SocketFundamentalClass::construct(State *state, std::vector<Node *> &args)
{
    SocketFundamentalObject *socketObj = new SocketFundamentalObject();
    if (!socketObj->isOpen())
        return Result<Value>("Socket creation failed");
    return Result<Value>(Value(socketObj));
}
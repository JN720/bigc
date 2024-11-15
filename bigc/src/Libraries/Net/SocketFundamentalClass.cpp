#include "SocketFundamentalClass.h"
#include "SocketFundamentalObject.h"
#include "SocketFundamentalMethodNode.h"

SocketFundamentalClass::SocketFundamentalClass()
{
    addMethod("connect", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                         {
        if (args.size() != 2)
            return Result<Value>("socket connection requires 2 arguments: address and port");
        Wildcard val = args[0]->getValue(state).getValue();
        std::string **ipAddress = std::get_if<std::string *>(&val);
        if (!ipAddress || !*ipAddress)
            return Result<Value>("ip address must be a string");
        if ((*ipAddress)->empty())
            return Result<Value>("ip address cannot be empty");
        Wildcard portVal = args[1]->getValue(state).getValue();
        int *port = std::get_if<int>(&portVal);
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

    addMethod("receiveClient", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                               {
        if (args.size() != 1)
            return Result<Value>("client receive requires a client");
        Wildcard val = args[0]->getValue(state).getValue();
        if (int *clientFD = std::get_if<int>(&val))
            return socketObj->receiveClient(*clientFD);
        return Result<Value>("client file descriptor must be an integer"); }),
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

    addMethod("sendClient", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                            {
        if (args.size() != 2)
            return Result<Value>("client send requires 2 arguments: client file descriptor and data");
        
        Wildcard clientVal = args[0]->getValue(state).getValue();
        int *clientFD = std::get_if<int>(&clientVal);
        if (!clientFD)
            return Result<Value>("client file descriptor must be an integer");

        Wildcard dataVal = args[1]->getValue(state).getValue();
        std::string **dataStr = std::get_if<std::string *>(&dataVal);
        if (!dataStr)
            return Result<Value>("data must be a string");
        
        if ((*dataStr)->empty())
            return Result<Value>("data cannot be empty");

        return socketObj->sendClient(*clientFD, **dataStr); }),
              false, PUBLIC);

    addMethod("accept", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                        {
        if (args.size())
            return Result<Value>("socket accept does not take any arguments");
        return socketObj->accept(); }),
              false, PUBLIC);

    addMethod("bind", new SocketFundamentalMethodNode([](SocketFundamentalObject *socketObj, State &state, std::vector<Node *> &args) -> Result<Value>
                                                      {
        if (args.size() != 2)
            return Result<Value>("socket bind requires 2 arguments: address and port");
        Wildcard val1 = args[0]->getValue(state).getValue();
        std::string **ipAddress = std::get_if<std::string *>(&val1);
        if (!ipAddress)
            return Result<Value>("address must be a string");
        Wildcard val2 = args[1]->getValue(state).getValue();
        int *port = std::get_if<int>(&val2);
        if (!port)
            return Result<Value>("port must be an integer");
        return socketObj->bind(**ipAddress, *port); }),
              false, PUBLIC);
}

Result<Value> SocketFundamentalClass::construct(State *state, std::vector<Node *> &args)
{
    SocketFundamentalObject *socketObj = new SocketFundamentalObject(this);
    if (!socketObj->isOpen())
        return Result<Value>("Socket creation failed");
    return Result<Value>(Value(socketObj));
}
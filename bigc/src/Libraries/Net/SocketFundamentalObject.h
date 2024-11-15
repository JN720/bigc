#ifndef SOCKET_FUNDAMENTAL_OBJECT_H
#define SOCKET_FUNDAMENTAL_OBJECT_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../FundamentalObject.h"

class SocketFundamentalObject : public FundamentalObject
{
public:
    SocketFundamentalObject(ClassDefinitionInterface *objClass);
    ~SocketFundamentalObject();

    bool isOpen() const;
    Result<Value> connect(std::string address, int port);
    Result<Value> send(const std::string &data);
    Result<Value> receive();
    Result<Value> listen(int backlog);
    Result<Value> accept();
    void close();
    Result<Value> bind(const std::string &address, int port);

private:
    int socketFD;
    struct sockaddr_in serverAddr;
};

#endif // SOCKET_FUNDAMENTAL_OBJECT_H

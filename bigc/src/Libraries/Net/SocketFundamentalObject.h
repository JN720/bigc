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
    SocketFundamentalObject();
    ~SocketFundamentalObject();

    bool isOpen() const;
    Result<Value> connect(const std::string &address, int port);
    Result<Value> send(const std::string &data);
    Result<Value> receive();
    Result<Value> listen(int backlog);
    void close();

private:
    int socketFD;
    struct sockaddr_in serverAddr;
};

#endif // SOCKET_FUNDAMENTAL_OBJECT_H

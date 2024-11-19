#pragma once
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../FundamentalObject.h"

#if defined(BUILD_CPU) || defined(BUILD_GPU)
class SocketFundamentalObject : public FundamentalObject
{
public:
    SocketFundamentalObject(ClassDefinitionInterface *objClass);
    ~SocketFundamentalObject();

    bool isOpen() const;
    Result<Value> connect(std::string address, int port);
    Result<Value> send(const std::string &data);
    Result<Value> sendClient(int clientFD, const std::string &data);
    Result<Value> receive();
    Result<Value> receiveClient(int clientFD);
    Result<Value> listen(int backlog);
    Result<Value> accept();
    void close();
    Result<Value> bind(const std::string &address, int port);

private:
    int socketFD;
    struct sockaddr_in serverAddr;
};
#endif // BUILD_CPU || BUILD_GPU

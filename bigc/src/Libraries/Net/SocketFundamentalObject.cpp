#include "SocketFundamentalObject.h"

SocketFundamentalObject::SocketFundamentalObject(ClassDefinitionInterface *objClass)
{
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    this->objClass = objClass;
}

SocketFundamentalObject::~SocketFundamentalObject()
{
    close();
}

bool SocketFundamentalObject::isOpen() const
{
    return socketFD >= 0;
}

Result<Value> SocketFundamentalObject::connect(std::string address, int port)
{
    if (socketFD < 0)
        return Result<Value>("Socket is not initialized");
    if (port <= 0 || port > 65535)
        return Result<Value>("Port must be in the range 1-65535");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr);

    if (::connect(socketFD, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        return Result<Value>("Connection failed");
    }
    return Result<Value>(Value(true));
}

Result<Value> SocketFundamentalObject::send(const std::string &data)
{
    ssize_t bytesSent = ::send(socketFD, data.c_str(), data.size(), 0);
    if (bytesSent < 0)
    {
        return Result<Value>("Send failed");
    }
    return Result<Value>(Value(bytesSent));
}

Result<Value> SocketFundamentalObject::sendClient(int clientFD, const std::string &data)
{
    ssize_t bytesSent = ::send(clientFD, data.c_str(), data.size(), 0);
    if (bytesSent < 0)
    {
        return Result<Value>("Send failed to client");
    }
    return Result<Value>(Value(bytesSent));
}

Result<Value> SocketFundamentalObject::receive()
{
    char buffer[1024];
    ssize_t bytesReceived = ::recv(socketFD, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0)
    {
        return Result<Value>("Receive failed");
    }
    buffer[bytesReceived] = '\0';
    return Result<Value>(Value(new std::string(buffer)));
}

Result<Value> SocketFundamentalObject::receiveClient(int clientFD)
{
    char buffer[1024];
    ssize_t bytesReceived = ::recv(clientFD, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0)
    {
        return Result<Value>("Receive failed from client");
    }
    buffer[bytesReceived] = '\0';
    return Result<Value>(Value(new std::string(buffer)));
}

Result<Value> SocketFundamentalObject::listen(int backlog)
{
    if (::listen(socketFD, backlog) < 0)
    {
        return Result<Value>("Listen failed");
    }
    return Result<Value>(Value(true));
}

void SocketFundamentalObject::close()
{
    if (isOpen())
    {
        ::close(socketFD);
    }
}

Result<Value> SocketFundamentalObject::accept()
{
    int clientFD = ::accept(socketFD, nullptr, nullptr);
    if (clientFD < 0)
    {
        return Result<Value>("Accept failed");
    }
    return Result<Value>(Value(clientFD));
}

Result<Value> SocketFundamentalObject::bind(const std::string &address, int port)
{
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr);

    if (::bind(socketFD, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        return Result<Value>("Bind failed");
    }
    return Result<Value>(Value(true));
}

#include "NetLibrary.h"
#include "SocketFundamentalClass.h"
#include "SocketOperations.h"
#include "../../ClassNode.h"

namespace libnet
{

    Registry *init()
    {
        Registry *registry = new Registry();

        // Register the Socket fundamental class
        registry->registerVariable("Socket", Value(new ClassNode(new SocketFundamentalClass())));

        // Register socket operations
        // registry->registerVariable("connect", Value(new ConnectFunction()));
        // registry->registerVariable("send", Value(new SendFunction()));
        // registry->registerVariable("receive", Value(new ReceiveFunction()));
        // registry->registerVariable("closeSocket", Value(new CloseSocketFunction()));

        return registry;
    }
}

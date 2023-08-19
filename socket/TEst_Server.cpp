#include "sockTcpServer.hpp"
#include <iostream>

int main(void)
{
    printf("action\n");
    sockTcp server;
    server.tcpBind();
    server.tcpListen();
    server.tcpAccept();
    char buffer[1024] = {"                       "};
    // char buffer_Save[1024] {};
    server.tcpRead(buffer);
    printf("message from client: %s", buffer);
    return 0;
}

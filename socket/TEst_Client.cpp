#include "sockTcpClient.hpp"

int main(void) {
    sockTcp_client client;
    client.tcpConnect();
    char *buffer = "this is a Test from client\n";
    client.tcpSend(buffer);
}
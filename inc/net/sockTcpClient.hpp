#ifndef _SOCKTCPCLIENT_HPP_
#define _SOCKTCPCLIENT_HPP_
#include "string.h"
#include "sockBase.hpp"

class sockTcp_client : public sockBase
{
    public:
        sockTcp_client(const char* ip = "127.0.0.1", int port = 8888) {
            this->sockFd = socket(AF_INET, SOCK_STREAM, 0);
            this->sockAddr.sin_addr.s_addr = inet_addr(ip);
            this->sockAddr.sin_family = AF_INET;
            this->sockAddr.sin_port = htons(port);
        }

        void tcpConnect() {
            if ((connect(this->sockFd, (struct sockaddr*)&this->sockAddr, sizeof(this->sockAddr))) < 0) {
                error_put("connect");
                return;
            } else {
                printf("connect success in %s\n", __FILE__);
                return;
            }
        }

        void tcpSend(const char* buffer) {
            if (send(this->sockFd, buffer, strlen(buffer), 0) < 0) {
                error_put("send");
                return;
            } else {
                printf("send success\n");
            }
        }
    private:

};

#endif _SOCKTCPCLIENT_HPP_
#ifndef _SOCKTCPSERVER_HPP_
#define _SOCKTCPSERVER_HPP_
#pragma once
#include <string.h>
#include "sockBase.hpp"
#include <vector>

class sockTcp : public sockBase{
    public:
    sockTcp() {
        int opt = 1;
        this->sockFd = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(this->sockFd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(int));
    }

    void tcpBind(const char* ipAddr = "127.0.0.1", int port = 8888) {
        this->sockAddr.sin_addr.s_addr = inet_addr(ipAddr);
        this->sockAddr.sin_family = AF_INET;
        this->sockAddr.sin_port = htons(8888);

        if (bind(this->sockFd, (const sockaddr*)&this->sockAddr, sizeof(this->sockAddr)) < 0) {
            error_put("Bind");
            return;
        } else {
            printf("bind success!\n");
        }
    }

    void tcpListen() {
        if (listen(this->sockFd, 5) < 0) {      //listen queen size is 6
            error_put("Listen");
            return;
        } else {
            printf("listen success!\n");
        }
    }

    void tcpAccept() {
        sockaddr_in clientAddr;
        socklen_t clientAddrlen = sizeof(clientAddr);

        while (1) {
            this->connectFd.push_back(accept(this->sockFd, (struct sockaddr*)&this->sockAddr, &clientAddrlen));
            if (connectFd.back() < 0) {
                error_put("Accept");
                return;
            } else {
                printf("connect(accept) success\n");
                return;
            }
        }
    }

    int& getVectordata(int data = -1) {
        if (data == -1) {
            return connectFd.back();
        } else {
            return connectFd[data];
        }
    }

    void tcpRead(char* buffer, int index = -1, int readLen = 1024) {
        if (!(index & -1) && connectFd.size() > index) {
            read(connectFd[index], buffer, readLen);
        } else {
            read(connectFd.back(), buffer, readLen);
        }
    }

    void tcpSend(char* buffer, int index = -1) {
        if (index != -1 && connectFd.size() > index) {
            send(connectFd[index], buffer, strlen(buffer), 0); 
        } else {
            send(connectFd.back(), buffer, strlen(buffer), 0);
        }
    }

    void tcpConnect(const char* ipAddr = "127.0.0.1", int port = 8888) {
        this->sockAddr.sin_addr.s_addr = inet_addr(ipAddr);
        this->sockAddr.sin_family = AF_INET;
        this->sockAddr.sin_port = htons(8888);
        if (connect(this->sockFd, (struct sockaddr*)&this->sockAddr, sizeof(this->sockAddr)) == -1) {
            error_put("Connect");
        } else {
            printf("connect success");
        }
    }

    void initServerSocket(const char* ip = "127.0.0.1", int port = 8888) {
        this->tcpBind(ip, port);
        this->tcpListen();
        this->tcpAccept();
    }
        
    private:
        std::vector<int> connectFd;
};

#endif _SOCKTCPSERVER_HPP_
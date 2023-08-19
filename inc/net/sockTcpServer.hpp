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
        }
    }

    void tcpListen() {
        if (listen(this->sockFd, 5) < 0) {      //listen queen size is 6
            error_put("Listen");
            return;
        }
    }

    bool tcpAccept(sockTcp& clientSocket) {
        sockaddr_in clientAddr;
        socklen_t clientAddrlen = sizeof(clientAddr);
        int clientFd = ::accept(sockFd, nullptr, nullptr);
        if (clientFd != -1) {
            clientSocket.setSocketFd(clientFd);
            return true;
        }
        return false;
    }

    ssize_t send(const void* data, size_t size) {
        return ::send(sockFd, data, size, 0);
    }

    ssize_t receive(void* buffer, size_t size) {
        return ::recv(sockFd, buffer, size, 0);
    }

    void setSocketFd(int fd) {
        this->sockFd = fd;
    }

    std::string getAddr() {
        return std::string(inet_ntoa(this->sockAddr.sin_addr));
    }

    // int& getVectordata(int data = -1) {
    //     if (data == -1) {
    //         return connectFd.back();
    //     } else {
    //         return connectFd[data];
    //     }
    // }

    // void tcpRead(char* buffer, int index = -1, int readLen = 1024) {
    //     if (!(index & -1) && connectFd.size() > index) {
    //         read(connectFd[index], buffer, readLen);
    //     } else {
    //         read(connectFd.back(), buffer, readLen);
    //     }
    // }

    // void tcpSend(char* buffer, int index = -1) {
    //     if (index != -1 && connectFd.size() > index) {
    //         send(connectFd[index], buffer, strlen(buffer), 0); 
    //     } else {
    //         send(connectFd.back(), buffer, strlen(buffer), 0);
    //     }
    // }

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
    }
        
    private:
        // std::vector<int> connectFd;
};

#endif _SOCKTCPSERVER_HPP_
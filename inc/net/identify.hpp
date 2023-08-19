#ifndef _IDENTIFY_HPP_
#define _IDENTIFY_HPP_
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

struct header_identify {
    uint16_t magic = 101;
    uint16_t size;
    uint8_t type;
    std::string token;
};

class ServerSocket_identify {
public:
    ServerSocket_identify(int port = 8080) {
        serverSockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSockfd == -1) {
            std::cerr << "Error creating server socket." << std::endl;
            return;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << "Error binding server socket." << std::endl;
            close(serverSockfd);
            return;
        }

        if (listen(serverSockfd, 10) == -1) {
            std::cerr << "Error listening for clients." << std::endl;
            close(serverSockfd);
            return;
        }
    }

    bool isTokenValid(const std::string token) {
        if (token.size()!=16)
        {
            return false;
        }
        auto time = (uint32_t)std::time(nullptr);
        uint32_t tokenTime = 0;
        for (int i = 0; i < 16; i++) {
            tokenTime |= (uint32_t)token[i] << (i * 2);
        }
        if (time - tokenTime > 60)
        {
            return false;
        }
        return true;
    }


    void generateToken() {
        uint8_t token[16];
        uint32_t time = (uint32_t)std::time(nullptr);
        for (int i = 0; i < 16; i++) {
            token[i] = (uint8_t)(time >> (i * 2));
        }
        this->head.token = std::string(reinterpret_cast<const char *>(token), 16);

    }

    bool headerDecode(std::string& str) {
        if (*reinterpret_cast<const uint16_t *>((str.substr(0, 2)).c_str()) != 101) return false;
        head.size = *reinterpret_cast<const uint16_t *>((str.substr(2, 4)).c_str());
        head.type = *reinterpret_cast<const uint8_t *>((str.substr(4, 6)).c_str());
        if (this->isTokenValid(str.substr(6, 6 + 16))) return true;
        return false;
    }

    void socketServerRun() {
        ServerSocket_identify serverSocket(8080);

        while (true) {
            int clientSockfd = serverSocket.acceptConnection();
            if (clientSockfd != -1) {
                std::string receivedData = serverSocket.receiveData(clientSockfd);
                std::cout << "Received from client: " << receivedData << std::endl;
                if (!this->headerDecode(receivedData)) continue;
                if (!(head.type == 1 || head.type == 3)) continue;
                bool isSuccess = false;
                std::string respond;
                respond = respond.append(reinterpret_cast<const char *>(head.magic), sizeof(uint16_t));
                head.size = 1;
                if (head.type == 3) {
                    respond = respond.append(reinterpret_cast<const char *>(head.size), sizeof(uint16_t));
                    respond = respond.append(reinterpret_cast<const char *>(++head.type), sizeof(uint8_t));
                    this->generateToken();
                    respond = respond.append(head.token);
                } else if (head.type == 1) {
                    respond = respond.append(reinterpret_cast<const char *>(head.size), sizeof(uint16_t));
                    respond = respond.append(reinterpret_cast<const char *>(++head.type), sizeof(uint8_t));
                    this->generateToken();
                    respond = respond.append(head.token);
                }
                serverSocket.sendData(clientSockfd, respond);

                close(clientSockfd); // 关闭客户端连接
            }
        }
    }

    int acceptConnection() {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSockfd = accept(serverSockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSockfd == -1) {
            std::cerr << "Error accepting client connection." << std::endl;
        }
        return clientSockfd;
    }

    bool sendData(int sockfd, const std::string& data) {
        if (send(sockfd, data.c_str(), data.size(), 0) == -1) {
            std::cerr << "Error sending data." << std::endl;
            return false;
        }
        return true;
    }

    std::string receiveData(int sockfd, int bufferSize = 1024) {
        char buffer[bufferSize];
        memset(buffer, 0, bufferSize);
        int bytesRead = recv(sockfd, buffer, bufferSize - 1, 0);
        if (bytesRead == -1) {
            std::cerr << "Error receiving data." << std::endl;
            return "";
        }
        return std::string(buffer, bytesRead);
    }

private:
    int serverSockfd;
    struct sockaddr_in serverAddr;
    header_identify head;
};

int main() {


    return 0;
}


#endif _IDENTIFY_HPP_
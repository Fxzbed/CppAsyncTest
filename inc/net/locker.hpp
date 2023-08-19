#ifndef LOCKER_HPP_
#define LOCKER_HPP_
#include "sockTcpClient.hpp"
#include "tokenGenerator.hpp"
#include <spdlog/spdlog.h>
class locker : public sockTcp_client
{
private:
    TokenGenerator_16 token;
    /* data */
public:
    locker(const char* ip = "127.0.0.1", int port = 8888) {
        this->sockFd = socket(AF_INET, SOCK_STREAM, 0);
        this->sockAddr.sin_addr.s_addr = inet_addr(ip);
        this->sockAddr.sin_family = AF_INET;
        this->sockAddr.sin_port = htons(port);
        token.generateNewToken();
    }

    void buildLockerHeader(std::string& str, uint8_t type, uint16_t size) {
        uint16_t magic = 514;
        str = str.append(reinterpret_cast<const char *>(magic), sizeof(uint16_t));
        str = str.append(reinterpret_cast<const char *>(size), sizeof(uint16_t));
        str = str.append(reinterpret_cast<const char *>(type), sizeof(uint8_t));
        this->token.generateNewToken();
        std::string token_;
        this->token.getCurrentToken(token_);
        str += token_;
    }

    void recvLocker(std::string& str, int recvBytes) {
        char buf[1024];
        if (read(this->sockFd, buf, recvBytes) < 0) {
            spdlog::warn("locker recv failed");
        } else {
            str.clear();
            str = buf;
        }
    }

    void sendLocker(std::string str) {
        if (send(this->sockFd, str.c_str(), str.size(), 0) < 0) {
            spdlog::warn("locker send failed");
        }
    }

    void generateToken_locker(std::string& str) {
        this->token.getCurrentToken(str);
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

    ~locker() {
        close(this->sockFd);
    }
};

#endif LOCKER_HPP_
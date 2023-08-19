#ifndef _TCPSOCKET_EPOLLSERVER_HPP_
#define _TCPSOCKET_EPOLLSERVER_HPP_
#include "/root/CodeWSL/CppAsyncTest/socket/sockTcpServer.hpp"
#include <spdlog/spdlog.h>
#include "msgPackbase.hpp"
#include <sys/epoll.h>


// struct clientAddrpack {
//     int ret;
//     sockaddr_in* clientAddr = nullptr;

//     clientAddrpack(int ret_ = -1) { ret = ret_; }
//     clientAddrpack(int ret_ = -1, sockaddr_in& clientAddr_) { 
//         ret = ret_;
//         clientAddr = new(sockaddr_in);
//         *clientAddr = clientAddr_; 
//     }

//     clientAddrpack& operator =(clientAddrpack& clientAddrpack_) {
//         std::swap(this->clientAddr, clientAddrpack_.clientAddr);
//         std::swap(this->ret, clientAddrpack_.ret);
//         return *this;
//     }

//     clientAddrpack(clientAddrpack& clientAddrpack_) {
//         std::swap(this->clientAddr, clientAddrpack_.clientAddr);
//         std::swap(this->ret, clientAddrpack_.ret);
//     }

//     ~clientAddrpack() {
//         if (this->clientAddr != nullptr) {
//             delete clientAddr;
//         }
//     }
// };

class epollServer : public sockTcp
{
    public:
        epollServer(const char* ip = "127.0.0.1", int port = 8888, int connectSize_ = 1024) {
            this->connectSize = connectSize_;
            this->epollEvlists = new(epoll_event[connectSize]);
            this->sockFd = socket(AF_INET, SOCK_STREAM, 0);

            this->tcpBind(ip, port);
            this->tcpListen();

            epollFd = epoll_create(connectSize);
            if (this->epollFd < 0) {
                perror("epoll create");
                return;
            } else {
                // printf("epoll create success\n");
                this->epollEv.events = EPOLLIN;
                this->epollEv.data.fd = this->sockFd;
                this->ret = epoll_ctl(this->epollFd, EPOLL_CTL_ADD, this->sockFd, &this->epollEv);
                if (this->ret < 0) {
                    perror("epoll_ctl");
                    return;
                }
            }
        }

        std::string getClientAddr() {
            return std::string(inet_ntoa(this->clientAddr.sin_addr));
        }

        std::string getClientPort () {
            return std::to_string(ntohs(this->clientAddr.sin_port));
        }

        bool getRdyret() {
            this->ret = epoll_wait(this->epollFd, this->epollEvlists, connectSize, 5000);
            if (this->ret < 0) {
                perror("epoll_wait");
                return false;
            } else if (0 == this->ret) {
                printf("epoll timeout\n");
                return false;
            } 
            return true;
        }

        int epollCtl(int i) {
            this->rdyFd = this->epollEvlists[i].data.fd;
            if (this->epollEvlists[i].events == EPOLLIN) {
                if (this->rdyFd == this->sockFd) {
                    socklen_t addrLen = sizeof(sockaddr_in);
                    this->connectFd_ = accept(this->sockFd, (struct sockaddr*)&clientAddr, &addrLen);
                    if (this->connectFd_ == -1) {
                        perror("new connect accept");
                        return -1;
                    } 
                    spdlog::info("new client in! Addr: " + this->getClientAddr() + this->getClientPort());
                    // printf("New connect in!\nAddr: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                    this->epollEv.events = EPOLLIN;
                    this->epollEv.data.fd = this->connectFd_;
                    if (this->ret = epoll_ctl(this->epollFd, EPOLL_CTL_ADD, this->connectFd_, &this->epollEv)) {
                        perror("connectFd_ epoll_ctl");
                        return -1;
                    }
                    return -1;
                } 
                return 1;
            }
            return -1;
        }

        void epollSend(std::string respond) {
            if (send(this->rdyFd, respond.c_str(), respond.size(), 0) == -1) {
                spdlog::warn("epoll send wrong!");
            }
        }

        void epollRead(char* msg, ssize_t& bytesRead) {
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            bytesRead = read(this->rdyFd, buf, 1024);   //test
            if (bytesRead == -1) {
                perror("epoll read");
                return;
            } else if (bytesRead == 0) {
                spdlog::info("client quit! Addr: " + std::string(inet_ntoa(clientAddr.sin_addr)));
                this->epollEv.events = EPOLLIN;
                this->epollEv.data.fd = this->rdyFd;
                this->ret = epoll_ctl(this->epollFd, EPOLL_CTL_DEL, rdyFd, &this->epollEv);
                if (this->ret == -1) {
                    perror("epoll ctl");
                }
                close(this->rdyFd);
            } else {
                strcpy(msg, buf);
            }
            return;
        }

        ~epollServer() {
            close(this->sockFd);
        }

        int ret;

    private:
        int epollFd;
        int connectFd_;
        int connectSize;
        int rdyFd;
        epoll_event epollEv;
        epoll_event *epollEvlists;
        sockaddr_in clientAddr;
};

#endif _TCPSOCKET_EPOLLSERVER_HPP_
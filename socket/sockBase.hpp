#ifndef _SOCKBASE_HPP_
#define _SOCKBASE_HPP_
#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string>

#define error_put(errorMsg) {printf("%s Error in line of %d in file: %s\n", errorMsg, __LINE__, __FILE__);}

class sockBase
{
    public:
        sockBase() {}

        void closeSock() {
            shutdown(this->sockFd, SHUT_RDWR);
            close(this->sockFd);
        }

        virtual ~sockBase() {}

    protected:
        /* data */
        sockaddr_in sockAddr;
        int sockFd;
};

#endif _SOCKBASE_HPP_

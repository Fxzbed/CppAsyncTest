#pragma once
#ifndef _MSGPACK_HPP_
#define _MSGPACK_HPP_
#include <cstdio>
#include <string.h>
struct bufferBase
{
    char *buffer;

    bufferBase(const char* buffer_) {
        buffer = new char[strlen(buffer)];
        strcpy(buffer, buffer_);
    }

    bufferBase() {
        buffer = new(char[1024]);
        memset(this->buffer, 0, strlen(this->buffer));
    }

    size_t bufferLen() {return strlen(this->buffer);}

    void clear() { memset (buffer, 0, strlen(buffer)); }
    
    ~bufferBase() { delete buffer;}
    /* data */
};


#endif _MSGPACK_HPP_
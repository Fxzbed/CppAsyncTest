#include "tcpSocket_epollServer.hpp"

int main(void) {
    epollServer server;
    printf("server listening...\n");
    while (1)
    {
        if (server.getRdyret()) {
            bufferBase bufferServer;
            for (int i = 0; i < server.ret; i ++) {
                if (server.epollCtl(i) > 0) {
                    server.epollRead(bufferServer);
                }
            }
        }
    }
}

// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <unistd.h>
// #include <sys/types.h>          /* See NOTES */
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netinet/ip.h> /* superset of previous */
// #include <arpa/inet.h>

// #include <sys/epoll.h>

// int main()
// {
//     int listenfd;
//     int connfd;
//     int ret;
//     int i;
//     int j;
//     char buf[128];
//     struct sockaddr_in srvaddr;
//     struct sockaddr_in cltaddr;
//     socklen_t addrlen;

//     listenfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (-1 == listenfd) {
//         perror("socket");
//         return -1;
//     }
//     printf("create socket success !\n");

//     memset(&srvaddr, 0, sizeof(srvaddr));
//     srvaddr.sin_family = AF_INET;
//     srvaddr.sin_port = htons(8888);
// // #if 0
//     // inet_aton("192.168.5.135", &(srvaddr.sin_addr));
// // #else
//     srvaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
// // #endif
//     ret = bind(listenfd, (const struct sockaddr *)&srvaddr, sizeof(srvaddr));
//     if (-1 == ret) {
//         perror("bind");
//         return -1;
//     }
//     printf("srv bind success !\n");

//     ret = listen(listenfd, 1024);
//     if (-1 == ret) {
//         perror("listen");
//         return -1;
//     }
//     printf("listen success\n");

//     int epfd;

//     /* 创建一个epoll实例 */
//     epfd = epoll_create(1024);    //1024表示的是某一个时间点期望检测到的最大个数；
//     if (-1 == epfd) {
//         perror("epoll_create");
//         return -1;
//     }

//     /* 将所要关心的文件描述符listenfd添加到eoll实例中 */
//     struct epoll_event event;    //需要添加的事件和文件描述符；
//     event.events = EPOLLIN;        //事件
//     event.data.fd = listenfd;    //文件描述符
//     ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &event);
//     if (-1 == ret ) {
//         perror("epoll_ctl");
//         return -1;
//     }

//     struct epoll_event events[1024];    //用来存储检测到的事件和文件描述符的集合（小于等于epoll实例所能处理最大数）
//     while(1)
//     {
//         ret = epoll_wait(epfd, events, 1024, 5000);    //循环检测；
//         if (ret == -1) {
//             perror("epoll");
//             return -1;
//         }
// // #if 1
//         else if (0 == ret) {
//             printf("epoll timeout ....\n");
//             continue;
//         }
// // #endif

//         for (i = 0; i < ret; i++)
//         {    //对准备就绪的事件和文件描述符进行一一轮询；
//             int fd = events[i].data.fd;    
//             if (events[i].events == EPOLLIN)
//             {    //判断是否为读事件
//                 if (fd == listenfd)
//                 {    //判断是否为监听套接字，如果是监听套接字则建立新的通信连接；
//                     addrlen = sizeof(struct sockaddr_in);
//                     connfd = accept(listenfd, (struct sockaddr*)&cltaddr, &addrlen);
//                     if (-1 == connfd)
//                     {
//                         perror("accept");
//                         return -1;
//                     }
//                     printf("IP : %s -- PORT : %d\n", inet_ntoa(cltaddr.sin_addr) , ntohs(cltaddr.sin_port));
//                     printf("accept connfd = %d success \n", connfd);
//                     /* 连接成功则将通信套接字添加到epoll实例中 */
//                     event.events = EPOLLIN;
//                     event.data.fd = connfd;
//                     ret = epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &event);
//                     if (-1 == ret ) {
//                         perror("epoll_ctl");
//                         return -1;
//                     }
//                 }
//                 else
//                 {    //如果是通信套接字，则处理客户端的数据请求。
//                     memset(buf, 0, sizeof(buf));
//                     ret = read(fd, buf, sizeof(buf));
//                     if (-1 == ret) {
//                         perror("read");
//                         return -1;
//                     } else if (0 == ret) {
//                         printf("client quit\n");
//                         /* 将退出的套接字文件描述符从epoll实例中清除 */
//                         event.events = EPOLLIN;
//                         event.data.fd = fd;
//                         ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
//                         if (-1 == ret ) {
//                             perror("epoll_ctl");
//                             return -1;
//                         }
//                         close(fd);
//                         continue;
//                     }
//                     printf("buf : %s\n", buf);
//                 }
//             }
//         }  
//     }   
//     close(listenfd);
//     return 0;
// }
#include "ctype.h"
#include "cstdio"
#include "stdio.h"
#include "string.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/epoll.h"
#include "errno.h"
#include "unistd.h"

#define MAX_LINE 80
#define SERV_PORT 8888
#define OPEN_MAX 1024

int main(int argc, char *argv[]) {
    int i, j, maxi, listenfd, connfd, sockfd;
    int nready, efd, res;
    ssize_t n;
    char buf[MAX_LINE], str[INET_ADDRSTRLEN];

    socklen_t clilen;
    int clients[OPEN_MAX];
    struct sockaddr_in cliaddr{}, servaddr{};
    struct epoll_event tep, ep[OPEN_MAX];
    // 網絡socket初始化
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    int opt = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) {
        perror("opt");
    }
    if (-1 == bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) {
        perror("bind");
    }
    if (-1 == listen(listenfd, 20)) {
        perror("listen");
    }

    puts("listen ok");
    for (i = 0; i < OPEN_MAX; i++) {
        clients[i] = -1;
    }

    maxi = -1; // 后面数据初始化赋值时，数据初始化为-1
    efd = epoll_create(OPEN_MAX); // 创建epoll句柄，底层其实是创建了一个红黑数
    if (efd == -1) {
        perror("epoll_create");
    }

    // 添加监听套接字
    tep.events = EPOLLIN;
    tep.data.fd = listenfd;
    res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep); //添加监听套接字， 即注册
    printf("efd = %d\n", efd);
    if (res == -1) {
        perror("epoll_ctl");
    }
    for (;;) {
        nready = epoll_wait(efd, ep, OPEN_MAX, -1);
        if (nready == -1) {
            perror("epoll wait");
        }
        //如果事件发生, 开始处理数据
        for (i = 0; i < nready; i++) {
            if (!(ep[i].events & EPOLLIN)) {
                continue;
            }
            // 若处理的时间和文件描述符相等，开始处理数据
            if (ep[i].data.fd == listenfd) { //判断发生的事件是不是来自监听套接字
                // 接收客户端
                clilen = sizeof(cliaddr);
                connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
                printf("connfd = %d\n", connfd);
                printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
                       ntohs(cliaddr.sin_port));
                for (j = 0; j < OPEN_MAX; j++) {
                    if (clients[j] < 0) {
                        // 将通信套接字存放到client
                        clients[j] = connfd;
                        break;
                    }
                }
                // 是否到达最大值，保护判断
                if (j == OPEN_MAX) {
                    perror("too many clients");
                }
                // 更新client下标
                if (j > maxi) {
                    maxi = j;
                }
                // 添加通信套接字到树（底层是红黑树）上
                tep.events = EPOLLIN;
                tep.data.fd = connfd;
                res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
                if (res == -1) {
                    perror("epoll_ctl");
                }

            } else {
                sockfd = ep[i].data.fd;
                n = read(sockfd, buf, MAX_LINE);
                if (n == 0) {
                    //将client中对应fd数据值恢复为-1
                    for (j = 0; j <= maxi; j++) {
                        if (clients[j] == sockfd) {
                            clients[j] = -1;
                            break;
                        }
                    }
                    // 删除树节点
                    res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
                    if(res == -1){
                        perror("epoll_ctl");
                    }
                    close(sockfd);
                    printf("client[%d] closed connection\n", j);
                } else {
                    printf("recive client[%d]'s data: %s\n", j, buf);
                    for (j = 0; j < n; j++){
                        buf[j] = toupper(buf[j]); // 简单转为大写
                    }
                    write(sockfd, buf, n); // 回送给客户端
                }
            }
        }

    }
    close(listenfd);
    close(efd);
    puts("close fd.");
    return 0;

}
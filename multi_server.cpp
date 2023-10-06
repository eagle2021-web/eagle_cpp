#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    unsigned short port = 8888;  // 服务器端口

    char on = 1;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // 创建套接字

    if (sockfd < 0) {
        perror("socket");
        exit(-1);
    }

    // 配置本地网络信息
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 端口复用
    int err_log = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    // 绑定
    if (err_log == -1) {
        perror("setsockopt");
        close(sockfd);
        exit(1);
    }

    err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if (err_log != 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    err_log = listen(sockfd, 10);  // 监听，套接字变为被动
    if (err_log != 0) {
        perror("listen");
        close(sockfd);
        exit(1);
    }

    printf("Server is waiting for client...\n");

    while (1) {
        // 主进程循环等待客户端的连接
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);

        if (connfd < 0) {
            perror("accept");
            close(sockfd);
            exit(1);
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            close(connfd);
            close(sockfd);
            exit(-1);
        } else if (pid == 0) {
            // 子进程接收客户端的信息，并返回给客户端
            close(sockfd);  // 关闭监听套接字，这个套接字是从父进程继承过来的

            char recv_buf[1024] = {0};
            int recv_len = 0;

            // 打印客户端的 IP 地址和端口号
            char cli_ip[INET_ADDRSTRLEN] = {0};
            inet_ntop(AF_INET, &(client_addr.sin_addr), cli_ip, sizeof(cli_ip));
            printf("Client IP: %s, Port: %d\n", cli_ip, ntohs(client_addr.sin_port));

            // 循环接收数据
            while ((recv_len = recv(connfd, recv_buf, sizeof(recv_buf), 0)) > 0) {
                printf("Received message: %s\n", recv_buf);

                // 给客户端返回数据
                send(connfd, recv_buf, recv_len, 0);
            }

            printf("Client closed connection!\n");
            close(connfd);  // 关闭已连接套接字
            exit(0);  // 子进程结束
        } else if (pid > 0) {
            // 父进程
            close(connfd);  // 关闭已连接套接字
        }
    }

    close(sockfd);

    return 0;
}
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cassert>
#include <errno.h>
#include <iostream>
#define BUF_LEN 300

typedef struct sockaddr SOCKADDR;

int main() {
    std::cout<<11<< std::endl;
    int err, i, iRes;
    char on = 1;
    int sockSrv = socket(AF_INET, SOCK_STREAM, 0); // 创建一个套接字，用于监听客户端的连接
    assert(sockSrv >= 0);
    struct linger so_linger;
    so_linger.l_onoff = 1;
    so_linger.l_linger = 0;
    setsockopt(sockSrv, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
    // 允许地址的立即重用
    setsockopt(sockSrv, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    sockaddr_in addrSrv{};
    addrSrv.sin_addr.s_addr = inet_addr("192.168.0.131");
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(8000); // 使用端口8000
    bind(sockSrv, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR)); // 绑定
    listen(sockSrv, 5); // 监听

    sockaddr_in addrClient{};
    int len = sizeof(SOCKADDR);
    while (true) {
        printf("- wait for client -\n");
        // 从连接请求队列中取出排在最前的一个客户端请求，如果队列为空就阻塞
        int sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, (socklen_t *)&len);

        char sendBuf[100] = "";
        for (i = 0; i < 10; i++) {
            sprintf(sendBuf, "N0.%d Welcome to the server. What is 1+1? (client IP: %s)\n", i + 1, inet_ntoa(addrClient.sin_addr));
            // 组成字符串
            send(sockConn, sendBuf, strlen(sendBuf), 0); // 发送字符串给客户端
            memset(sendBuf, 0, sizeof(sendBuf));
        }

        // 数据发送结束，调用shutdown()函数声明不再发送数据，此时客户端仍可以接收数据
        iRes = shutdown(sockConn, SHUT_WR);
        if (iRes == -1) {
            printf("shutdown failed with error: %d\n", errno);
            close(sockConn);
            return 1;
        }

        // 发送结束，开始接收客户端发来的信息
        char recvBuf[BUF_LEN];
        // 持续接收客户端数据，直到对方关闭连接
        do {
            //在 recv 函数中，参数 0 用于指示接收操作不使用任何特殊的标志。这表示在接收数据时没有任何特殊的要求或选项（例如，不启用 MSG_OOB（带外数据）或 MSG_WAITALL（等待完全接收）标志）。当 recv 函数的第四个参数为 0 时，它的行为与不指定任何标志时的行为相同。它将尽可能接收数据，然后立即返回接收到的字节数。
            //因此，iRes = recv(sockConn, recvBuf, BUF_LEN, 0); 语句的作用是从与套接字 sockConn 关联的连接中接收数据并将其存储在 recvBuf 中，最多接收 BUF_LEN 字节的数据。函数返回接收到的字节数，并将其赋值给 iRes 变量。
            iRes = recv(sockConn, recvBuf, BUF_LEN, 0);
            if (iRes > 0) {
                printf("Recv %d bytes.\n", iRes);
                for (i = 0; i < iRes; i++) {
                    // 格式化
                    printf("%c", recvBuf[i]);
                }
                printf("\n");
            }
            else if (iRes == 0){
                printf("The client has closed the connection.\n");
            }
            else {
                printf("recv failed with error: %d\n", errno);
                close(sockConn);
                return 1;
            }
        } while (iRes > 0);

        close(sockConn); // 关闭和客户端通信的套接字
        printf("%d", 111);
//        puts("Continue monitoring?(y/n)");
//        char ch[2];
//        scanf("%1s", ch); // 从控制台读取一个字符
//        if (ch[0] != 'y') // 如果不是'y'就退出循环
//            break;
    }

    close(sockSrv); // 关闭监听套接字
    return 0;
}

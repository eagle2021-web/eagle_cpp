#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int serv_len, err;
    char on = 1;

    // 创建一个套接字，用于监听客户端的连接
    int sockSrv = socket(AF_INET, SOCK_STREAM, 0);
    // 允许地址的立即重用
    setsockopt(sockSrv, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    struct sockaddr_in serv, addrsrv;
    memset(&addrsrv, 0, sizeof(struct sockaddr_in));
    addrsrv.sin_addr.s_addr = inet_addr("192.168.0.131");
    addrsrv.sin_family = AF_INET;
    addrsrv.sin_port = htons(8000);

    // 使用端口8000
    if (-1 == bind(sockSrv, (struct sockaddr *)&addrsrv, sizeof(struct sockaddr))) {
        printf("bind fail: %d!\r\n", errno);
        return -1;
    }

    // 获取本地套接字地址
    serv_len = sizeof(struct sockaddr_in);
    getsockname(sockSrv, (struct sockaddr *)&serv, (socklen_t *)&serv_len);

    // 打印套接字地址里的IP地址和端口值，以便让客户端知道
    printf("server has started, ip=%s, port=%d\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port));

    listen(sockSrv, 5);

    sockaddr_in addrClient;
    int len = sizeof(sockaddr_in);

    while (1) {
        printf("\nwait for client\n");

        // 从连接请求队列中取出排在最前的一个客户端请求，如果队列为空就阻塞
        int sockConn = accept(sockSrv, (struct sockaddr *)&addrClient, (socklen_t *)&len);

        char sendBuf[100];
        sprintf(sendBuf, "Welcome client (%s) to Server!", inet_ntoa(addrClient.sin_addr));

        // 组成字符串
        send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);

        char recvBuf[100];
        recv(sockConn, recvBuf, 100, 0);

        // 接收客户端信息
        printf("Receive client's msg: %s\n", recvBuf);

        // 打印收到的客户端信息
        close(sockConn);

        // 关闭和客户端通信的套接字
        puts("continue to listen? (y/n)");
//        char ch[2];
//        scanf("%1s", ch);

        // 读控制台的两个字符，包括回车符
//        if (ch[0] != 'y') {
//            // 如果不是y就退出循环
//            break;
//        }
    }

    close(sockSrv);

    // 关闭监听套接字
    return 0;
}
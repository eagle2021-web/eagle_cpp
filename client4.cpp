#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_LEN 250

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

int main() {
    int err;
    u_long argp;
    char szMsg[] = "Hello, server, I have received your message.";
    int sockClient = socket(AF_INET, SOCK_STREAM, 0); // 新建一个套接字

    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.s_addr = inet_addr("192.168.0.131"); // 服务器的 IP 地址
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(8888); // 服务器的监听端口

    err = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)); // 向服务器发出连接请求
    if (-1 == err) // 判断连接是否成功
    {
        printf("Failed to connect to the server: %d\n", errno);
        getchar();
        return 0;
    }

    char recvBuf[BUF_LEN]; // BUF_LEN 是 250
    int i, cn = 1, iRes;
    int leftlen = 50 * 111; // 这个 5550 是通信双方约定的

    while (leftlen > BUF_LEN) {
        // 接收来自服务器的信息，每次最大只能接收 BUF_LEN 个数据，具体接收多少未知
        iRes = recv(sockClient, recvBuf, BUF_LEN, 0);
        if (iRes > 0) {
            printf("\nNo.%d: Recv %d bytes:", cn++, iRes);
            for (i = 0; i < iRes; i++)
                // 打印本次接收到的数据
                printf("%c", recvBuf[i]);
            printf("\n");
        } else if (iRes == 0) {
            // 对方关闭连接
            puts("\nThe server has closed the connection.\n");
        } else {
            printf("Recv failed: %d\n", errno);
            close(sockClient);
            return -1;
        }
        leftlen = leftlen - iRes;
    }

    if (leftlen > 0) {
        iRes = recv(sockClient, recvBuf, leftlen, 0);
        if (iRes > 0) {
            printf("\nNo.%d: Recv %d bytes:", cn++, iRes);
            for (i = 0; i < iRes; i++)
                // 打印本次接收到的数据
                printf("%c", recvBuf[i]);
            printf("\n");
        } else if (iRes == 0) {
            // 对方关闭连接
            puts("\nThe server has closed the connection.\n");
        } else {
            printf("Recv failed: %d\n", errno);
            close(sockClient);
            return -1;
        }
    }

    // 开始向服务器端发送数据
    char sendBuf[100];
    sprintf(sendBuf, "Hi, Server, I've finished receiving the data."); // 组成字符串
    send(sockClient, sendBuf, strlen(sendBuf) + 1, 0); // 发送字符串给服务器端

    memset(sendBuf, 0, sizeof(sendBuf));
    puts("Sending data to the server is completed.");

    close(sockClient); // 关闭套接字

//    getchar();
    return 0;
}
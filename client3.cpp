#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUF_LEN 300

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

int main() {
    int err;
    long argp;
    char szMsg[] = "Hollo, server, I have received your message.";
    int sockClient = socket(AF_INET, SOCK_STREAM, 0); // 新建一个套接字

    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.s_addr = inet_addr("192.168.0.131"); // 服务器的IP地址
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(8000); // 服务器的监听端口
//    printf("addrSrv.sin_addr.s_addr 地址：%p\n", &(addrSrv.sin_addr.s_addr));
//    printf("addrSrv.sin_family 地址：%p\n", &(addrSrv.sin_family));
//    printf("addrSrv.sin_port 地址：%p\n", &(addrSrv.sin_port));
    err = connect(sockClient, (SOCKADDR *)&addrSrv, sizeof(SOCKADDR)); // 向服务器发出连接请求
    if (-1 == err) { // 判断连接是否成功
        printf("Failed to connect to the server. Please check whether the server is started.\n");
        getchar();
        return 0;
    }

    char recvBuf[BUF_LEN];
    int i, cnt, iRes;
    do {
        iRes = recv(sockClient, recvBuf, BUF_LEN, 0); // 接收来自服务器的信息
        if (iRes > 0) {
            printf("\nRecv %d bytes:\n", iRes);
            for (i = 0; i < iRes; i++)
                printf("%c", recvBuf[i]);
//            printf("\n");
        } else if (iRes == 0) { // 对方关闭连接
            puts ("\nThe server has closed the send connection.\n");
        } else {
            printf("recv failed: %d\n", errno);
            close(sockClient);
            return 1;
        }
    } while (iRes > 0);

    // 开始向服务器发送数据
    char sendBuf[100];
    for (i = 0; i < 10; i++) {
        sprintf(sendBuf, "NO. %d I'm the client, 1 + 1 = 2\n", i + 1); // 组成字符串
        send(sockClient, sendBuf, strlen(sendBuf) + 1, 0); // 发送字符串给服务器
        memset(sendBuf, 0, sizeof(sendBuf));
    }

    puts("Sending data to the server is completed.");
    close(sockClient); // 关闭套接字
//    getchar();
    return 0;
}
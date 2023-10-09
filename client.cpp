#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

int main() {
    int err;
    int sockClient = socket(AF_INET, SOCK_STREAM, 0); // 新建一个套接字
    char msg[] = "hi,server"; // 要发送给服务器端的消息

    sockaddr_in addrSrv{};
    addrSrv.sin_addr.s_addr = inet_addr("192.168.0.131"); // 服务器的IP地址
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(8888); // 服务器的监听端口

    // 向服务器发出连接请求
    err = connect(sockClient, (struct sockaddr *)&addrSrv, sizeof(struct sockaddr));
    if (-1 == err) { // 判断连接是否成功
        printf("Failed to connect to the server. Please check whether the server is started\n");
        return 0;
    }

    char recvBuf[100];
//    recv(sockClient, recvBuf, 100, 0); // 接收来自服务器的信息
//    printf("received server's msg: %s\n", recvBuf); // 打印收到的信息
    char empty_msg[] = "adfsdf";
    send(sockClient, msg, strlen(empty_msg) + 1, 0); // 向服务器发送信息
    recv(sockClient, recvBuf, 100, 0); // 接收来自服务器的信息
    printf("received server's msg: %s\n", recvBuf); // 打印收到的信息
    close(sockClient); // 关闭套接字
//    getchar(); // 要用户输入字符才结束程序
    return 0;
}
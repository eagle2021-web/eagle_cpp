#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

int main()
{
    int sfp;
    struct sockaddr_in s_add;
    unsigned short portnum = 8022;
    struct sockaddr_in serv = { 0 };
    char on = 1;
    int serv_len = sizeof(serv);

    sfp = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sfp)
    {
        printf("socket fail!\n");
        return -1;
    }
    printf("socket ok!\n");

    // 打印未绑定前的地址
    printf("ip=%s, port=%d\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port));

    // 允许地址立即重用
    setsockopt(sfp, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    memset(&s_add, 0, sizeof(struct sockaddr_in));
    s_add.sin_family = AF_INET;
    s_add.sin_addr.s_addr = inet_addr("192.168.0.131"); // 这个IP地址必须是本机上有的
    s_add.sin_port = htons(portnum);

    if (-1 == bind(sfp, (struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
        printf("bind fail:%d!\n", errno);
        return -1;
    }
    printf("bind ok!\n");

    // 获取本地套接字地址，serv_len是输入输出参数，输入时serv_len = sizeof(serv)
    getsockname(sfp, (struct sockaddr *)&serv, (socklen_t *)&serv_len);

    // 打印套接字地址里的IP地址和端口值
    printf("ip=%s, port=%d\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port));

    return 0;
}
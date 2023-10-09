#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <fcntl.h>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <iostream>
using std::vector;

void errExit() {
    getchar();
    exit(-1);
}

const char resp[] = "HTTP/1.1 200\r\n\
Content-Type: application/json\r\n\
Content-Length: 13\r\n\
Date: Thu, 2 Aug 2021 04:02:00 GMT\r\n\
Keep-Alive: timeout=60\r\n\
Connection: keep-alive\r\n\r\n\
[HELLO WORLD]\r\n\r\n";

int main() {
    const int port = 8888;
    int sd, ret;

    // 创建套接字
    sd = socket(AF_INET, SOCK_STREAM, 0);
    fprintf(stdout, "created socket\n");
    if (sd == -1) {
        errExit();
    }

    int opt = 1;
    // 重用地址
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) {
        errExit();
    }
    fprintf(stdout, "socket opt set\n");

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addrLen = sizeof(addr);
    if (bind(sd, (sockaddr *) &addr, sizeof(addr)) == -1)
        errExit();
    fprintf(stdout, "socket binded\n");

    if (listen(sd, 1024) == -1)

        errExit();
    fprintf(stdout, "socket listen start\n");
    // 套接字创建完毕，初始化监听列表
    int currentFdNum = 1;
    vector<pollfd> fds(100);
    fds[0].fd = sd;
    fds[0].events = POLLIN;
    nfds_t nfds = 1;
    int timeout = -1;
    fprintf(stdout, "polling\n");

    while (1) {
        // 执行poll操作
        ret = poll(fds.data(), nfds, 30000);
        std::cout << "while循環" << std::endl;
        fprintf(stdout, "poll returned with ret value: %d\n", ret);
        if (ret == -1)
            errExit();
        else if (ret == 0) {
            fprintf(stdout, "return no data\n");
        } else { // ret > 0
            // 检查是否有新客户端建立连接
            printf("有新客户端建立连接\n");
            if (fds[0].revents & POLLIN) {
                printf("有新客户端建立连接\n");
                sockaddr_in childAddr{};
                socklen_t childAddrLen;
                int childSd = accept(sd, (sockaddr *) &childAddr, &(childAddrLen));

                if (childSd == -1)
                    errExit();
                fprintf(stdout, "child got\n");

                // 设置非阻塞
                int flags = fcntl(childSd, F_GETFL);
                if (fcntl(childSd, F_SETFL, flags | O_NONBLOCK) == -1)
                    errExit();
                fprintf(stdout, "child set nonblock\n");

                // 添加到列表
                fds[currentFdNum].fd = childSd;
                fds[currentFdNum].events = (POLLIN | POLLRDHUP | POLLOUT);
                nfds++;
                currentFdNum++;
                fprintf(stdout, "child: %d pushed to poll list\n", currentFdNum - 1);
            }

            // 客户端读写
            for (int i = 1; i < currentFdNum; i++) {
                if (fds[i].revents & (POLLHUP | POLLRDHUP | POLLNVAL)) {
                    // 客户端描述符关闭
                    fprintf(stdout, "child: %d shutdown\n", i);
                    close(fds[i].fd);
                    fds[i].events = 0;
                    fds[i].fd = -1;
                    continue;
                }

                // 读取请求数据
                if (fds[i].revents & POLLIN) {
                    char buffer[17] = "";
                    while (true) {
                        ret = read(fds[i].fd, buffer, sizeof(buffer));
                        fprintf(stdout, "read on: %d returned with value: %d\n", i, ret);
                        if (ret == 0) {
                            fprintf(stdout, "read returned 0(EOF) on: %d, breaking\n", i);
                            break;
                        }
                        if (ret == -1) {
                            const int tmpErrno = errno;
                            // tmpErrno 是一个变量，表示最近的错误代码。
                            // EWOULDBLOCK 和 EAGAIN 都是表示非阻塞操作时可能返回的错误码。它们表示操作暂时无法完成，需要稍后再次尝试。
                            // 因此，if (tmpErrno == EWOULDBLOCK || tmpErrno == EAGAIN) 这行代码的作用是检查 tmpErrno 是否为 EWOULDBLOCK 或 EAGAIN，
                            // 如果是，则执行 if 语句块中的代码。通常在非阻塞 I/O 操作中，当返回这两个错误码时，我们可以通过稍后再次尝试来处理操作。
                            if (tmpErrno == EWOULDBLOCK || tmpErrno == EAGAIN) {
                                fprintf(stdout, "read would block, stop reading\n");
                                // 可以监听写事件了
                                fds[i].events |= POLLOUT;
                                break;
                            } else {
                                errExit();
                            }
                        }
                    }
                }
                // 写事件，发送响应
                if (fds[i].revents & POLLOUT) {
                    std::cout << 22222 << std::endl;
                    ret = write(fds[i].fd, resp, sizeof(resp));
                    std::cout << 333 << std::endl;
                    fprintf(stdout, "write on: %d returned with value: %d\n", i, ret);
                    if (ret == -1) {
                        errExit();
                    }
                    fds[i].events &= ~(POLLOUT);
                }
            }
        }
    }

    return 0;
}

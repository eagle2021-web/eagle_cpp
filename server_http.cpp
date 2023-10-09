//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <pthread.h>
//#define ISspace(x) isspace((int)(x))  //宏定义，是否是空格
//#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"
//
//void error_die(const char* error_message) {
//    perror(error_message);
//    exit(EXIT_FAILURE);
//}
//void bad_request(int client){
//    printf("no handle bad_request.\n");
//}
//void cannot_execute(int client){
//    printf("no handle cannot_execute.\n");
//}
//int get_line(int sock, char *buf, int size) {// buf是存放数据的缓冲区，size是缓冲区
//    int i = 0;
//    char c = '\0';
//    int n;
//    while ((i < size - 1) && (c != '\n')) {
//        n = recv(sock, &c, 1, 0);
//        if (n > 0) {
//            if (c == '\r') {
//                n = recv(sock, &c, 1, MSG_PEEK);
//                if ((n > 0) && (c == '\n')) {
//                    recv(sock, &c, 1, 0);
//                } else {
//                    c = '\n';
//                }
//                buf[i] = c;
//                i++;
//            }
//        } else {
//            c = '\n';
//        }
//    }
//    buf[i] = '\0';
//    return (i);
//}
//
//void execute_cgi(int client, const char *path, const char *method, const char *query_string) {
//    char buf[1024]; // 缓冲区
//    int cgi_output[2];
//    int cgi_input[2];
//    pid_t pid;
//    int status, i;
//    char c;
//    int numchars = 1; // 读取的字符数
//    int content_length = -1; // HTTP的content_length
//    // 首先需要根据请求是GET还是POST来分别进行处理
//    buf[0] = 'A';
//    buf[1] = '\0';
//
//    // 忽略大小写比较字符串
//    if (strcasecmp(method, "GET") == 0) { // 如果是GET，那么就忽略剩余的请求头
//        // 读取数据，把整个header都读取并丢弃
//        while ((numchars > 0) && strcmp("\n", buf))
//            numchars = get_line(client, buf, sizeof(buf));
//    } else { // 如果是POST，那么就需要读出请求长度即Content-Length
//        numchars = get_line(client, buf, sizeof(buf));
//        while ((numchars > 0) && strcmp("\n", buf)) {
//            // 如果是POST请求，就需要得到Content-Length
//            // Content-Length：这个字符串一共长为15位，
//            // 所以取出头部一句后，将第16位设置结束符，进行比较第16位置为结束
//            buf[15] = '\0';
//            // 使用＼0进行分割
//            if (strcasecmp(buf, "Content-Length:") == 0) { // HTTP请求的特点
//                content_length = atoi(
//                        &(buf[16])); // 内存从第17位开始就是长度，将17位开始的所有字符串转成整数就是content_length
//                numchars = get_line(client, buf, sizeof(buf));
//                break;
//            }
//        }
//        // 如果请求长度不合法（比如根本就不是数字），那么就报错，即没有找到content_length
//        if (content_length == -1) {
//            bad_request(client);
//            return;
//        }
//    }
//
//    sprintf(buf, "HTTP/1.0 200 OK\r\n");
//    send(client, buf, strlen(buf), 0);
//
//// 建立output管道
//    if (pipe(cgi_output) < 0) {
//        cannot_execute(client);
//        return;
//    }
//
//// 建立input管道
//    if (pipe(cgi_input) < 0) {
//        cannot_execute(client);
//        return;
//    }
//
//// fork进程，子进程用于执行cgi，父进程用于接收数据以及发送子进程处理的回复数据
//    if ((pid = fork()) < 0) {
//        cannot_execute(client);
//        return;
//    }
//
//    if (pid == 0) { /* child: CGI script */
//        char meth_env[255];
//        char query_env[255];
//        char length_env[255];
//
//        // 子进程输出重定向到output管道的1端
//        dup2(cgi_output[1], 1);
//
//        // 子进程输入重定向到input管道的0端
//        dup2(cgi_input[0], 0);
//
//        // 关闭无用管道口
//        close(cgi_output[0]);
//        close(cgi_input[1]);
//
//        // cgi环境变量
//        sprintf(meth_env, "REQUEST_METHOD=%s", method);
//        putenv(meth_env);
//
//        if (strcasecmp(method, "GET") == 0) {
//            sprintf(query_env, "QUERY_STRING=%s", query_string);
//            putenv(query_env);
//        } else { /* POST */
//            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
//            putenv(length_env);
//        }
//
//        // 替换执行path
//        execl(path, path, NULL);
//        exit(0); // 退出子进程，管道被破坏，但是父进程还在往里面写东西，触发SIGPIPE信号
//    } else { /* parent */
//        // 关闭无用管道口
//        close(cgi_output[1]);
//        close(cgi_input[0]);
//
//        if (strcasecmp(method, "POST") == 0) {
//            for (i = 0; i < content_length; i++) {
//                // 得到POST请求数据，写到input管道中，供子进程使用
//                recv(client, &c, 1, 0);
//                write(cgi_input[1], &c, 1);
//            }
//        }
//
//        // 从output管道读到子进程处理后的信息，然后send出去
//        while (read(cgi_output[0], &c, 1) > 0)
//            send(client, &c, 1, 0);
//
//        // 完成操作后关闭管道
//        close(cgi_output[0]);
//        close(cgi_input[1]);
//
//        waitpid(pid, &status, 0); // 等待子进程返回
//    }
//}
//void headers(int client, const char *filename)
//{
//    char buf[1024];
//    (void)filename; /* could use filename to determine file type */
//
//    strcpy(buf, "HTTP/1.0 200 OK\r\n");
//    send(client, buf, strlen(buf), 0);
//    strcpy(buf, SERVER_STRING);
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "Content-Type: text/html\r\n");
//    send(client, buf, strlen(buf), 0);
//    strcpy(buf, "\r\n");
//    send(client, buf, strlen(buf), 0);
//}
//void not_found(int client)
//{
//    char buf[1024];
//
//    sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, SERVER_STRING);
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "Content-Type: text/html\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "your request because the resource specified\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "is unavailable or nonexistent.\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "</BODY></HTML>\r\n");
//    send(client, buf, strlen(buf), 0);
//}
//void serve_file(int client, const char *filename)
//{
//    FILE *resource = NULL;
//    int numchars = 1;
//    char buf[1024];
//
//    //默认字符
//    buf[0] = 'A'; buf[1] = '\0';
//    while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
//        numchars = get_line(client, buf, sizeof(buf));
//
//    resource = fopen(filename, "r");
//    if (resource == NULL)
//        not_found(client);
//    else
//    {
//        headers(client, filename);
//        cat(client, resource);
//    }
//    fclose(resource);
//}
//int startup(u_short *port)
//{
//    int httpd = 0;
//    struct sockaddr_in name;
//    httpd = socket(PF_INET, SOCK_STREAM, 0);
//    if (httpd == -1)
//        error_die("socket");
//    memset(&name, 0, sizeof(name));
//    name.sin_family = AF_INET;
//    name.sin_port = htons(*port);
//    name.sin_addr.s_addr = htonl(INADDR_ANY);
//    //绑定socket
//    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
//        error_die("bind");
//    //如果端口没有设置，提供个随机端口
//    if (*port == 0)  /* if dynamically allocating a port */
//    {
//        socklen_t  namelen = sizeof(name);
//        if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
//            error_die("getsockname");
//        *port = ntohs(name.sin_port);
//    }
//    //监听
//    if (listen(httpd, 5) < 0)
//        error_die("listen");
//    return (httpd);
//}
//// 添加函数unimplemented，该函数返回给浏览器表明收到的HTTP请求所用的method不被支持，代码如下：
//void unimplemented(int client)
//{
//    char buf[1024];
//
//    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, SERVER_STRING);
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "Content-Type: text/html\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "</TITLE></HEAD>\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
//    send(client, buf, strlen(buf), 0);
//    sprintf(buf, "</BODY></HTML>\r\n");
//    send(client, buf, strlen(buf), 0);
//}
//
//int main() {
//    int server_sock = -1;
//    u_short port = 8888;  //监听端口，如果为0，则系统自动分配一个端口
//    int client_sock = -1;
//    struct sockaddr_in client_name;
//
//    //这边要为socklen_t类型
//    socklen_t client_name_len = sizeof(client_name);
//    pthread_t newthread;
//    //建立一个监听套接字，在对应的端口建立httpd服务
//    server_sock = startup(&port);
//    printf("httpd running on port %d\n", port);
//
//    while (1)  //无限循环
//    {
//        //阻塞等待客户端的连接请求
//        client_sock = accept(server_sock,   //返回一个已连接套接字
//                             (struct sockaddr *)&client_name,
//                             &client_name_len);
//        if (client_sock == -1)
//            error_die("accept");
//        //派生线程用accept_request函数处理新请求
//        //每次收到请求，创建一个线程来处理接收到的请求
//        //把client_sock转成地址作为参数传入pthread_create
//        if (pthread_create(&newthread, NULL, (void *)accept_request, (void *)
//                (intptr_t)client_sock) != 0)
//            perror("pthread_create");
//
//    }
//    close(server_sock);
//    return 0;
//}
//
//
int main(){

}
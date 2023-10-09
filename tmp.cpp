#include <sys/poll.h>
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;
int main(){
    vector<pollfd> fds(100);
    cout << fds.data()->events <<endl;
    cout << fds.data()->fd  <<endl;
    cout << fds.data()->revents <<endl;
    char buf[1024];
    buf[16] = "Content-Length:";
    // 使用＼0进行分割
    if (strcasecmp(buf, "Content-Length:") == 0){
        cout << 11111 <<endl;
    }

}
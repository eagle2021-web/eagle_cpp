#include <cstdio>
#include <iostream>
#include <string.h>
#include <cstdlib>

using namespace std;

struct MyData {
    int nLen;
    char* data;
};

int main() {
    int nLen = 10;
    char str[10] = "123456789";

    cout << "Size of MyData: " << sizeof(MyData) << endl;
    cout << "Size of char pointer: " << sizeof(char*) << endl;
    MyData *myData = (MyData *)malloc(sizeof(MyData));
    myData->nLen = nLen;
    myData->data = (char *)malloc(nLen);
    memcpy(myData->data, str, nLen);
    myData->data[nLen] = '\0'; // 添加字符串结尾的空字符
    cout << "myData's Data is: " << myData->data << endl;
    cout << "Size of MyData: " << sizeof(MyData) << endl;

    free(myData->data);
    free(myData);

    return 0;
}
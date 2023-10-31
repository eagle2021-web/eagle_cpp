```cpp
#include <iostream>
using namespace std;

int main() {

   // 输入不匹配
   int a;
   cout << "请输入一个整数: ";
   if (cin >> a) {
       cout << "输入的整数为: " << a << endl;
   } else {
       cerr << "输入的不是整数" << endl;
   }

   // 输入缓冲区留有残留数据
   int b;
   cout << "请输入一个整数: ";
   cin >> b;
   cin.ignore(100,'\n'); // 忽略剩下的100个字符，如果超过100个，则100以后的字符保留；或者遇到换行符，则换行符之后的字符保留
   cout << "请输入一个字符串: ";
   string str;
   getline(cin, str); // 从cin中读取一行，包含'\n'字符
   cout << "输入的整数为: " << b << endl;
   cout << "输入的字符串为: " << str << endl;

   // 输入流错误状态
   double c;
   cout << "请输入一个浮点数: ";
   cin >> c;
   if (cin.good()) {
       cout << "输入的浮点数为: " << c << endl;
   } else if (cin.eof()) {
       cerr << "已到文件结尾" << endl;
   } else {
       cerr << "输入失败" << endl;
   }

   // 输入流不可重用
   int d;
   cout << "请输入一个整数: ";
   cin >> d;
   cout << "输入的整数为: " << d << endl;
   // 无法再次读取d
   double e;
   cout << "请输入一个浮点数: ";
   cin >> e;
   cout << "输入的浮点数为: " << e << endl;

   return 0;
}
```


```c++
#include <iostream>
using namespace std;
int main() {
/*
 * 
在您的代码中，您使用 cin.getline(buf, 3, '\n') 读取了最多两个字符并存入 buf。这是因为 buf 的大小为 3，用于存放两个字符和一个空字符（字符串的结束标记）。

当您输入 “213 321”，由于 buf 的大小为 3，cin.getline() 只能读取 “21”，并将其存入 buf，同时将 “3 321\n” 留在了输入流中。这就是为何你看到 “21” 输出的原因。

接着，因为 “3 321\n” 仍在输入流中，当您调用 std::getline(std::cin, name) 时，name 实际上得到的是空字符串。因为在尝试读取 name 之前，输入流中立刻遇到了换行符（‘\n’），所以 getline 直接返回了，导致 name 是空字符串。

所以，输出 “Hello , nice to meet you.” 是因为 name 为空的原因。并且，输入流中会剩余 " 321\n"。

最后，cin.rdstate() 返回的状态表明 cin 是 failbit 的状态。这是因为当 cin.getline() 试图从包含 " 321\n" 的输入流中读取时，超出了 buf 的大小，所以设置了 failbit。

出于这个原因，一般推荐在处理用户的输入，特别是大小未知的输入时，使用 std::string 和 std::getline() 函数，而不是 char 数组和 cin.getline() 函数。因为 std::getline() 会自动处理溢出，并且可以避免这类问题。
 * */
    char buf[3];
    cin.getline(buf, 3, '\n');// 读行，最多3个字符，3个字符后的保留，所以可能会有\n?
    std::string name;
    std::getline(std::cin, name);
    std::cout << "Hello " << name << ", nice to meet you.\n";
    cout << buf << endl;
    // 获取 cin 的错误状态
    std::ios::iostate state = std::cin.rdstate();
    // 检查子状态位
    if (state & std::ios::failbit) {
        // 处理 failbit 错误
        std::cout << "输入格式错误" << std::endl;
    }
    if (state & std::ios::badbit) {
        // 处理 badbit 错误
        std::cout << "输入流发生致命错误" << std::endl;
    }
    if (state & std::ios::eofbit) {
        // 处理 eofbit 错误
        std::cout << "到达输入流末尾" << std::endl;
    }
}
```


### string
```c++
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
int main()
{
    // greet the user
    std::string name;
    std::cout << "What is your name? ";
    std::getline(std::cin, name);
    std::cout << "Hello " << name << ", nice to meet you.\n";
    int b = atoi(name.c_str());
    cout << b << endl;
    // read file line by line
    std::istringstream input;
    input.str("1\n2\n3\n4\n5\n6\n7\n");
    int sum = 0;
    for (std::string line; std::getline(input, line); )
        sum += std::stoi(line);
    std::cout << "\nThe sum is " << sum << ".\n\n";

    // use separator to read parts of the line
    std::istringstream input2;
    input2.str("a;b;c;d");
    for (std::string line; std::getline(input2, line, ';'); )
        std::cout << line << '\n';
}
```
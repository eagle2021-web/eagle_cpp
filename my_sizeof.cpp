#include <iostream>
using namespace std;

int main() {
    system("chcp 65001");
	cout << "short 类型所占内存空间为：" << sizeof(short) << endl;
	cout << "int 类型所占内存空间为：" << sizeof(int) << endl;
	cout << "long 类型所占内存空间为：" << sizeof(long) << endl;
	cout << "long long 类型所占内存空间为：" << sizeof(long long) << endl;
    int a = 1;
    int n_int = INT_MAX;
    // initialize n int to max int value
    short n_short = SHRT_MAX;
    // symbols defined in climits file
    long n_long = LONG_MAX;
    long long n_llong = LLONG_MAX;
    // sizeof operator yields size of type or of variable
    cout << "int is " << sizeof (int) << " bytes." << endl;
    cout << "short is " << sizeof n_short << " bytes." << endl;
    cout << "long is " << sizeof n_long << " bytes." << endl;
    cout << "long long is " << sizeof n_llong << " bytes." << endl;
    cout << endl;
    cout << "Maximum values:" << endl;
    cout << "int:"<<n_int << endl;
    cout << "short: " << n_short << endl;
    cout << "long: " << n_long << endl;
    cout << "long long: " << n_llong << endl << endl;
    cout << "Minimum int value = " << INT_MIN << endl;
    cout <<"Bits per byte ="<< CHAR_BIT << endl;
    return 0;
}
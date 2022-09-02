#include <iostream>
#include "my_const.h"
#include "my_sizeof.h"
using namespace std;
int main() {
	cout << "hello_world" << endl;
	// #######
	int a = abc();
	cout << "a=" << a << endl;
	// ##
	my_sizeof();
	system("pause");
	return 0;
}
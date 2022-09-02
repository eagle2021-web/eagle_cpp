// 1.宏常量
#define Day 7
#include <iostream>

int abc() {
	// Day = 14 // 错误，Day是常量，一旦修改就会报错
	std::cout << Day << std::endl;
	//2. const修饰的变量
	const int month = 12;
	//month = 14 //报错，变量是不可以修改的
	return Day;
}
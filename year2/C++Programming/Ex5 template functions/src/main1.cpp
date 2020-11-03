#include "task1.h"
#include <iostream>

int main () {
	double a = 2.9, b = 3.1;
	const char * str1 = "abc";
	const char * str2 = "abcc";

	std::cout << less(str1, str2) << std::endl;
	std::cout << less(a, b) << std::endl;

	return 0;
}
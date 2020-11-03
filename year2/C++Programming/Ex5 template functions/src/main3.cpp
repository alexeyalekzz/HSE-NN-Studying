#include <iostream>
#include "task3.h"

int main () {
	const int N{5};
	int arr[N] {10, 20, 30, 40, 1};

	std::cout << minArr(arr) << std::endl;

	return 0;
}
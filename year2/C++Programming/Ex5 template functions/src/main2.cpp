#include <iostream>
#include "task2.h"

int main () {
	const int n{10};
	double arr[n] = {1.2, 122.4, 12.6, 5, 0, 52.6, 9.9, 16, 6.8, 14};

	std::cout << averageArr(arr, n);

	return 0;
}
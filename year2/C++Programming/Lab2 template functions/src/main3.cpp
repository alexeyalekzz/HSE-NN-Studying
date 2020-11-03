#include <iostream>
#include <cstring>
#include "task3.h"


template<typename T>
T change (T a) {
	if (a > 10)
		return a - 10;
	else
		return a + 10;
}

int main () {

	int a[5] {3, 4, 1, 2, 5};

	map(a, change);

	for (size_t i{0}; i < 5; i++) {
		std::cout << a[i] << ' ';
	}

    return 0;
}
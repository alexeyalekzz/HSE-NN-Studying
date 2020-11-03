#include <iostream>
#include "task1.h"


int main() {

	int a[5] {3, 4, 1, 2, 5};
	msort(a, 5);

	for (auto n : a) {
		std::cout << n << ' ';
	}

	return 0;
}

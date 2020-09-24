#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "task1.h"

using namespace std;

int main() {
	int arr[LARGE_SIZE];
	int arr_copy[LARGE_SIZE];
	int SIZE;
	int iter = 50;

	double output_qsort[iter];
	double output_sort[iter];

	for (int i = 0; i < iter; i++) {

		srand(time(nullptr));
		SIZE = 2000 * (i + 1);
		// генерируем случайные данные
		generate( arr, arr + SIZE, rnd() );
		copy( arr, arr + SIZE, arr_copy );
		// засекаем время
		time_t start = clock();
		// выполняем сортировку, используя функцию qsort
		qsort( arr, SIZE, sizeof( int ), comp );
		output_qsort[i] = static_cast<double>( clock() - start ) / CLOCKS_PER_SEC;

		// снова засекаем время
		start = clock();
		// выполняем сортировку, используя алгоритм С++
		sort( arr_copy, arr_copy + SIZE );
		output_sort[i] = static_cast<double>( clock() - start ) / CLOCKS_PER_SEC;
	}

	cout << "C quick-sort time elapsed: ";
	for (auto t : output_qsort)
		cout << t << ' ';
	cout << '\n';

	cout << "C++ quick-sort time elapsed: ";
	for (auto t : output_sort)
		cout << t << ' ';

	return 0;

}
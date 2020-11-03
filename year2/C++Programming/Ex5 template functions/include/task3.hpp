#ifndef TASK1_TASK3_H
#define TASK1_TASK3_H

template<typename T, size_t N>
T minArr (T (& arr)[N]) {

	T min = arr[0];
	for (size_t i = 1; i < N; i++) {
		if (arr[i] < min)
			min = arr[i];
	}

	return min;
}

#endif //TASK1_TASK3_H

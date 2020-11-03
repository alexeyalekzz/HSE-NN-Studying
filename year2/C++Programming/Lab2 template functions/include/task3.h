#ifndef LAB2_TASK3_H
#define LAB2_TASK3_H

template<typename T, size_t N>
void map (T (&arr)[N], T change(T a) ) {

	for (size_t i{0}; i < N; i++) {
		arr[i] = change(arr[i]);
	}

}

#endif //LAB2_TASK3_H

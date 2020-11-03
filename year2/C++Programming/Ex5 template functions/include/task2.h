#ifndef TASK1_TASK2_H
#define TASK1_TASK2_H

template <typename T>
double averageArr (T * arr, int size) {

	double s{0};
	for (int i = 0; i < size; i++) {
		s+=arr[i];
	}
	return s/size;
}

#endif //TASK1_TASK2_H

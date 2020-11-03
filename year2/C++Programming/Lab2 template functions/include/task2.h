#ifndef LAB2_TASK2_H
#define LAB2_TASK2_H

template<typename T, size_t N>
T * createArr ( T fun() ) {

	T * temp = new T [N];
	for (size_t i = 0; i < N; i++) {

		temp[i] = fun();
	}

	return temp;
}

#endif //LAB2_TASK2_H

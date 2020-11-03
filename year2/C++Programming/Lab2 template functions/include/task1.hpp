#ifndef LAB2_TASK1_H
#define LAB2_TASK1_H

#include <cstring>

template <typename T>
void msort (T* arr, size_t size) {

	if (size > 1)
	{
		const size_t left = size / 2;
		const size_t right = size - left;

		msort(&arr[0], left);
		msort(&arr[left], right);

		T * temp = new T [size];

		size_t l = 0, r = left, i = 0;
		while (l < left || r < size)
		{
			if (arr[l] < arr[r])
			{
				temp[i++] = std::move(arr[l]);
				l++;
			}
			else
			{
				temp[i++] = std::move(arr[r]);
				r++;
			}

			if (l == left)
			{
				std::copy(std::make_move_iterator(&arr[r]), std::make_move_iterator(&arr[size]), &temp[i]);
				break;
			}
			if (r == size)
			{
				std::copy(std::make_move_iterator(&arr[l]), std::make_move_iterator(&arr[left]), &temp[i]);
				break;
			}
		}

		std::copy(std::make_move_iterator(temp), std::make_move_iterator(&temp[size]), arr);
	}
}
template<>
void msort (char** arr, size_t size ) {

	if (size > 1)
	{
		const size_t left = size / 2;
		const size_t right = size - left;

		msort(&arr[0], left);
		msort(&arr[left], right);

		char** temp = new char* [size];

		size_t l = 0, r = left, i = 0;
		while (l < left || r < size)
		{
			if (strlen(arr[l]) < strlen(arr[r]))
			{
				temp[i++] = arr[l];
				l++;
			}
			else
			{
				temp[i++] = arr[r];
				r++;
			}

			if (l == left)
			{
				std::copy(std::make_move_iterator(&arr[r]), std::make_move_iterator(&arr[size]), &temp[i]);
				break;
			}
			if (r == size)
			{
				std::copy(std::make_move_iterator(&arr[l]), std::make_move_iterator(&arr[left]), &temp[i]);
				break;
			}
		}

		std::copy(std::make_move_iterator(temp), std::make_move_iterator(&temp[size]), arr);
	}
}

#endif //LAB2_TASK1_H

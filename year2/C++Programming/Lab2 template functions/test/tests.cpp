#include "gtest/gtest.h"
namespace lab2
{
	#include "task1.h"
	#include "task2.h"
	#include "task3.h"
	
	template<class T>
	T gen()
	{
		static int t = 48;
		return t++;
	}

	template<>
	char* gen()
	{
		static unsigned t = 0;
		t++;
		char* s = new char[t + 1];
	
		for(size_t i = 0; i < t; i++)
			s[i] = '0' + i;
		s[t] = '\0';

		return s;
	}

	template<class T>
	T change(T val)
	{
		return val + 1;
	}

	template<>
	char* change(char* val)
	{
		size_t n = strlen(val);
		for(size_t i = 0; i < n; i++)
			val[i] = val[i] + 1;
		return val;
	}
}
#include <cstring>

// task1
TEST(lab2,task1_1)
{
	const size_t n = 6;
	int *val = new int[n]{5, 3, 2, 4, 1, 6};
	lab2::msort(val, n);
	int *expected = new int[n]{1, 2, 3, 4, 5, 6};
    
    for(size_t i = 0; i < n; i++)
		EXPECT_EQ(expected[i], val[i]);
	delete[] val;
	delete[] expected;
}

TEST(lab2,task1_2)
{
	const size_t n = 6;
	double val[n]{4.0, 3.0, 2.4, 2.5, 2.1, -1};
	lab2::msort(val, n);
    double expected[n]{-1, 2.1, 2.4, 2.5, 3.0, 4.0};
    
    for(size_t i = 0; i < n; i++)
		EXPECT_EQ(expected[i], val[i]);
}

TEST(lab2,task1_3)
{
	const size_t n = 6;
	char* val[n]{(char*)"ffff", (char*)"fff2", (char*)"f", (char*)"ff", (char*)"vvv", (char*)"fffrr"};
	lab2::msort(val, n);
    char* expected[n]{(char*)"f", (char*)"ff", (char*)"vvv", (char*)"fff2", (char*)"ffff", (char*)"fffrr"};
    
    for(size_t i = 0; i < n; i++)
		EXPECT_EQ(expected[i], val[i]);
}

// task2
TEST(lab2,task2_1)
{
	const size_t n = 5;
	int* arr;
	arr = lab2::createArr<int, n>(lab2::gen);

	int *expected = new int[n]{48, 49, 50, 51, 52};
    
    for(size_t i = 0; i < n; i++)
		EXPECT_EQ(expected[i], arr[i]);
	delete[] arr;
	delete[] expected;
}

TEST(lab2,task2_2)
{
	const size_t n = 5;
	char* arr;
	arr = lab2::createArr<char, n>(lab2::gen);

	char *expected = new char[n]{48, 49, 50, 51, 52};
    
    for(size_t i = 0; i < n; i++)
		EXPECT_EQ(expected[i], arr[i]);
	delete[] arr;
	delete[] expected;
}

TEST(lab2,task2_3)
{
	const size_t n = 5;
	char** arr;
	arr = lab2::createArr<char*, n>(lab2::gen);
	
	char** expected = new char*[n];
	for(size_t i = 0; i < n; i++)
	{
		expected[i] = new char[i + 2];
		for(size_t j = 0; j < i + 1; j++)
			expected[i][j] = '0' + j;
		expected[i][i + 1] = '\0';
	}
    
    for(size_t i = 0; i < n; i++)
	{	
		EXPECT_EQ(strlen(expected[i]), strlen(arr[i]));
		if (strcmp(expected[i], arr[i]))
			for(size_t j = 0; j <= strlen(expected[i]); j++)
				EXPECT_EQ(expected[j], arr[j]);
	}
	for(size_t i = 0; i < n; i++)
	{
		delete[] arr[i];
		delete[] expected[i];
	}
	delete[] arr;
	delete[] expected;
}

// task3
TEST(lab2,task3_1)
{
	const size_t n = 5;
	int arr[n]{1, 2, 3, 4, 5};
	lab2::map<int, n>(arr, lab2::change);

	int expected[n]{2, 3, 4, 5, 6};
    
    for(size_t i = 0; i < n; i++)
		EXPECT_EQ(expected[i], arr[i]);
}

TEST(lab2,task3_2)
{
	const size_t n = 5;
	double arr[5]{1, 2.2, 3.3, 4.4, 5};
	lab2::map<double, n>(arr, lab2::change);

	double expected[n]{2, 3.2, 4.3, 5.4, 6};
    
    for(size_t i = 0; i < n; i++)
		EXPECT_EQ(expected[i], arr[i]);
}

TEST(lab2,task3_3)
{
	const size_t n = 5;
	char arr[n]{'1', '2', '3', '4', '5'};
	lab2::map<char, n>(arr, lab2::change);

	char expected[n]{'2', '3', '4', '5', '6'};
    
    for(size_t i = 0; i < n; i++)
		EXPECT_EQ(expected[i], arr[i]);
}

TEST(lab2,task3_4)
{
	const size_t n = 5;
	char* arr[n];
	char* expected[n];
	for(size_t i = 0; i < n; i++)
	{
		arr[i] = new char[i + 2];
		expected[i] = new char[i + 2];
		for(size_t j = 0; j < i + 1; j++)
		{
			arr[i][j] = '0' + i;
			expected[i][j] = '1' + i;
		}
		arr[i][i + 1] = '\0';
		expected[i][i + 1] = '\0';
	}
	lab2::map<char*, n>(arr, lab2::change);

	for(size_t i = 0; i < n; i++)
	{	
		EXPECT_EQ(strlen(expected[i]), strlen(arr[i]));
		if (strcmp(expected[i], arr[i]))
			for(size_t j = 0; j <= strlen(expected[i]); j++)
				EXPECT_EQ(expected[j], arr[j]);
	}

	for(size_t i = 0; i < n; i++)
	{
		delete[] arr[i];
		delete[] expected[i];
	}
}
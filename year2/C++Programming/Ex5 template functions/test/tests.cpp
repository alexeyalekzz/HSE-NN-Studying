#include "gtest/gtest.h"
namespace Ex05
{
	#include "task1.h"
	#include "task2.h"
	#include "task3.h"
}
#include <cstring>

// task1
TEST(ex05,task1_1)
{
	int a=5,b=6;
	char x='a',y='b';
	bool result=Ex05::less(a,b);
	EXPECT_EQ(true,result);
	result=Ex05::less(x,y);
	EXPECT_EQ(true,result);
}
TEST(ex05,task1_2)
{
	const char *str1="abba";
	const char *str2="baab";
	bool result=Ex05::less(str2,str1);
	EXPECT_EQ(false,result);
}

// task2
TEST(ex05,task2_1)
{
	const size_t n = 6;
	int val[n]{4, 3, 2, 2, 2, -1};
	int result=Ex05::averageArr(val, n);
        int expected=2;
        EXPECT_EQ(expected, result);
}

// task3
TEST(ex05,task3_1)
{
	const size_t n = 6;
	int val[n]{4, 3, 2, 2, 2, -1};
	int result=Ex05::minArr(val);
        int expected=-1;
        EXPECT_EQ(expected, result);
}


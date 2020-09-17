#include "task1.h"

#include "gtest/gtest.h"

TEST (task1, test1_average) {
	ASSERT_DOUBLE_EQ(average(10, 5), 7.5);
}

TEST (task1, test2_average) {
	ASSERT_DOUBLE_EQ(average(1231.41, 32112.3), 16671.855);
}

TEST (task1, test3_good) {
	ASSERT_FALSE(good(23.1231211, 23.12312108));
}

TEST (task1, test4_good) {
	ASSERT_FALSE(good(23.1231211, 23.12312101));
}

TEST (task1, test5_improve) {
	ASSERT_DOUBLE_EQ(improve(2, 6), 2.5);
}

TEST (task1, test6_improve) {
	ASSERT_DOUBLE_EQ(improve(1, 20), 10.5);
}

TEST (task1, test7_iter) {
	ASSERT_DOUBLE_EQ(iter(1.41421, 2), 1.4142135623775818);
}

TEST (task1, test8_iter) {
	ASSERT_DOUBLE_EQ(iter(4, 16), 4.0);
}

TEST (task1, test9_calc) {
	ASSERT_DOUBLE_EQ(calc(2), 1.4142135623746899);
}

TEST (task1, test10_calc) {
	ASSERT_DOUBLE_EQ(calc(123), 11.090536506409418);
}
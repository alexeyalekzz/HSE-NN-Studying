// Copyright 2020 GHA Test Team
#include "MyStack.h"

#include <gtest/gtest.h>
#include <string>

TEST(MyStackTest, test1) {
  MyStack<int> st(5);
  EXPECT_EQ(true, st.isEmpty());
}
TEST(MyStackTest, test2) {
  MyStack<int> st(5);
  st.push(1);
  EXPECT_EQ(false, st.isEmpty());
}
TEST(MyStackTest, test3) {
  MyStack<double> st(5);
  st.push(2);
  st.pop();

  EXPECT_EQ(true, st.isEmpty());
}
TEST(MyStackTest, test4) {
  MyStack<int> st(5);
  st.push(14);

  EXPECT_EQ(14, st.pop());
}
TEST(MyStackTest, test5) {
  MyStack<int> st(5);
  st.push(14);

  EXPECT_EQ(14, st.get());
}
TEST(MyStackTest, test6) {
  MyStack<std::string> st(5);
  st.push("first");
  st.push("second");

  std::string res("second");
  EXPECT_EQ(res, st.get());
}
TEST(MyStackTest, test7) {
  MyStack<int> st(2);
  st.push(1);
  st.push(2);

  EXPECT_EQ(true, st.isFull());
}
TEST(MyStackTest, test8) {
  MyStack<int> st(3);
  st.push(1);
  st.push(2);

  EXPECT_EQ(false, st.isFull());
}
TEST(MyStackTest, test9) {
  MyStack<int> st(3);
  st.push(1);
  st.push(2);

  MyStack clone(st);

  while (!st.isEmpty())
    EXPECT_EQ(clone.pop(), st.pop());
}
TEST(MyStackTest, test10) {
  MyStack<int> st(3);

  EXPECT_THROW(st.pop(), std::out_of_range);
}
TEST(MyStackTest, test11) {
  MyStack<int> st(3);

  EXPECT_THROW(st.get(), std::out_of_range);
}
TEST(MyStackTest, test12) {
  MyStack<int> st(0);
  st.push(-1);

  EXPECT_EQ(-1, st.get());
}

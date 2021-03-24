// Copyright 2020 GHA Test Team
#include <gtest/gtest.h>
#include <string>
#include "Postfix.h"

TEST(PostfixTest, test1) {
  std::string inf = "2 + 2";
  std::string postf = infix2postfix(inf);
  std::string expected = "2 2 +";
  EXPECT_EQ(expected, postf);
}

TEST(PostfixTest, test2) {
  std::string inf = "2 + 6 * 3 / (4 - 2)";
  std::string postf = infix2postfix(inf);
  std::string expected = "2 6 3 * 4 2 - / +";
  EXPECT_EQ(expected, postf);
}

TEST(PostfixTest, test3) {
  std::string inf = "(2 + 8.3) * (6 - 3.2)";
  std::string postf = infix2postfix(inf);
  std::string expected = "2 8.3 + 6 3.2 - *";
  EXPECT_EQ(expected, postf);
}

TEST(PostfixTest, test4) {
  std::string inf = "(6 * (4 - 2) + 5) * (2.6 + 3 * 7) - 9";
  std::string postf = infix2postfix(inf);
  std::string expected = "6 4 2 - * 5 + 2.6 3 7 * + * 9 -";
  EXPECT_EQ(expected, postf);
}

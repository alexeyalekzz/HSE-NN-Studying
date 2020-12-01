// Copyright 2020 GHA Test Team
#include "Fraction.h"

#include <gtest/gtest.h>
#include <string>

TEST(FractionTest, test1) {
  Fraction a;
  EXPECT_EQ(0, a.getNumerator());
  EXPECT_EQ(1, a.getDenominator());
}

TEST(FractionTest, test2) {
  Fraction a(1, 2);
  EXPECT_EQ(1, a.getNumerator());
  EXPECT_EQ(2, a.getDenominator());
}

TEST(FractionTest, test3) {
  Fraction a(1, 2);
  Fraction b(a);
  EXPECT_EQ(1, b.getNumerator());
  EXPECT_EQ(2, b.getDenominator());
}

TEST(FractionTest, test4) {
  Fraction a(1, 2);
  Fraction b(a);
  Fraction c;
  c = a + b;
  EXPECT_EQ(1, c.getNumerator());
  EXPECT_EQ(1, c.getDenominator());
}

TEST(FractionTest, test5) {
  Fraction a(1, 2);
  Fraction b(-1, 2);
  Fraction c;
  c = a + b;
  EXPECT_EQ(0, c.getNumerator());
  EXPECT_EQ(1, c.getDenominator());
}

TEST(FractionTest, test6) {
  Fraction a(1, 2);
  Fraction b(1, 2);
  Fraction c;
  c = a - b;
  EXPECT_EQ(0, c.getNumerator());
  EXPECT_EQ(1, c.getDenominator());
}

TEST(FractionTest, test7) {
  Fraction a(1, 3);
  Fraction b(2, 3);
  Fraction c;
  c = a + b;
  EXPECT_EQ(1, c.getNumerator());
  EXPECT_EQ(1, c.getDenominator());
}

TEST(FractionTest, test8) {
  Fraction a(1, 2);
  Fraction b(1, 2);
  Fraction c;
  c = a * b;
  EXPECT_EQ(1, c.getNumerator());
  EXPECT_EQ(4, c.getDenominator());
}

TEST(FractionTest, test9) {
  Fraction a(1, 5);
  Fraction b(1, 2);
  Fraction c;
  c = a / b;
  EXPECT_EQ(2, c.getNumerator());
  EXPECT_EQ(5, c.getDenominator());
}

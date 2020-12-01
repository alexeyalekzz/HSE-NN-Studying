// Copyright (c) 2020 Alexey
//
// Created by Alexey on 30.11.2020.
//
#include <Fraction.h>
#include <iostream>

int main() {
  Fraction a(12, 15);
  Fraction b(1, 2);
  Fraction c = (a / b) * (a - b * Fraction(2, 3) + b / a);

  std::cout << c.getValue() << std::endl;

  try {
    Fraction c1 = c / Fraction(0);
  }
  catch (const std::runtime_error& e) {
    std::cout << e.what();
  }

  return 0;
}

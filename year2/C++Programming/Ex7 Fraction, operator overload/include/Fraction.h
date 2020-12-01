// Copyright (c) 2020 Alexey
//
// Created by Alexey on 30.11.2020.
//

#ifndef INCLUDE_FRACTION_H_
#define INCLUDE_FRACTION_H_

#include <string>

class Fraction {
 private:
  int numer, denom;
  void normalize();
 public:
  explicit Fraction(int = 0, int = 1);
  Fraction(const Fraction&) = default;
  [[nodiscard]] std::string getValue() const;
  [[nodiscard]] int getNumerator() const;
  [[nodiscard]] int getDenominator() const;

  Fraction operator+(Fraction const &a) const;
  Fraction operator+(Fraction const &&a) const;
  Fraction operator-(Fraction const &a) const;
  Fraction operator*(Fraction const &a) const;
  Fraction operator/(Fraction const &a) const;
  Fraction& operator=(Fraction const &a);
};

#endif  // INCLUDE_FRACTION_H_

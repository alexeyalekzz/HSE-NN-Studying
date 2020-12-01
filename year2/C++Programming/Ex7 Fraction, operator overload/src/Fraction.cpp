// Copyright (c) 2020 Alexey
//
// Created by Alexey on 30.11.2020.
//

#include <Fraction.h>
#include <stdexcept>

int NOD(unsigned int a, unsigned int b) {
  while (a && b) {
    if (a >= b)
      a = a % b;
    else
      b = b % a;
  }
  return static_cast<int>(a+b);
}

int NOK(int n, int b) {
  return n / NOD(n, b) * b;
}

Fraction::Fraction(int num, int den) : numer(num), denom(den) {
  normalize();
}

void Fraction::normalize() {
  if (denom == 0) {
    throw std::runtime_error("Divide by zero Exception!");
  } else if (denom < 0) {
    denom *= -1;
    numer *= -1;
  }

  int nod = NOD(std::abs(numer), std::abs(denom));
  numer /= nod;
  denom /= nod;
}

std::string Fraction::getValue() const {
  return std::string(std::to_string(numer)
                      + (denom != 1 ? '/' + std::to_string(denom) : ""));
}

int Fraction::getNumerator() const {
  return numer;
}

int Fraction::getDenominator() const {
  return denom;
}

Fraction Fraction::operator*(Fraction const &a) const {
  return Fraction(this->numer * a.numer, this->denom * a.denom);
}

Fraction Fraction::operator/(Fraction const &a) const {
  return *this * Fraction(a.denom, a.numer);
}

Fraction Fraction::operator+(Fraction const &a) const {
  int nok = NOK(this->denom, a.denom);
  return Fraction(this->numer * (nok / this->denom) + a.numer * (nok / a.denom),
                  nok);
}

Fraction Fraction::operator+(Fraction const &&a) const {
  return *this + a;
}

Fraction Fraction::operator-(Fraction const &a) const {
  return *this + Fraction(-a.numer, a.denom);
}

Fraction& Fraction::operator=(Fraction const &a) {
  if (this != &a) {
    this->numer = a.numer;
    this->denom = a.denom;
  }
  return *this;
}

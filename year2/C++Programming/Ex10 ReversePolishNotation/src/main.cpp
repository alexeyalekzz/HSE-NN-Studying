// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 11.01.2021.
//
#include <iostream>
#include <string>

#include "Postfix.h"

int main() {
  std::string infix("4 + 3 / 14 * (2 - 7) / (3 * (33 - 26)) / 41");

  std::cout << "Infix expression:   " << infix << std::endl
            << "Postfix expression: " << infix2postfix(infix) << std::endl;

  return 0;
}

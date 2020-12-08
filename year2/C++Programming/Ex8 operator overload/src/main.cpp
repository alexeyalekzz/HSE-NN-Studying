// Copyright (c) 2020 Alexey
//
// Created by Alexey on 07.12.2020.
//
#include <iostream>
#include "MyString.h"

int main() {
  MyString s1("Hello");
  MyString s2(" World");
  MyString s3("!");

  MyString hello = s1 + s2 + s3 * 5;

  std::cout << hello;

  return 0;
}

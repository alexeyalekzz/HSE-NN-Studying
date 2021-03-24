// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 11.01.2021.
//
#include "MyStack.h"
#include <iostream>

int main() {
  MyStack<int> s(10);
  for (int i = 0; !s.isFull(); i++) {
    s.push(i);
  }

  MyStack<int> reverse_s(10);
  while (!s.isEmpty()) {
    reverse_s.push(s.pop());
  }

  MyStack to_output(reverse_s);
  while (!to_output.isEmpty()) {
    std::cout << to_output.pop() << ' ';
  }
  std::cout << std::endl;

  try {
    auto temp = s.get();
  } catch (const std::out_of_range& e) {
    std::cout << e.what() << std::endl;  // "Get call on empty stack!"
  }

  return 0;
}


// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 11.01.2021.
//
#include <iostream>
#include "MyStack.h"
#include "Postfix.h"

bool priority(char a, char b) {
  if (a == '(' || ((a == '+' || a == '-') && (b == '*' || b == '/')))
    return false;
  else
    return true;
}

std::string infix2postfix(const std::string& exp) {
  MyStack<char> ops;
  std::string postfix;

  for (size_t i = 0; i < exp.size(); i++) {
    if (exp[i] >= '0' && exp[i] <= '9') {
      size_t numlen;

      auto space = exp.find(' ', i);
      auto p = exp.find(')', i);
      if (space != p)
        numlen = (space < p ? space : p) - i;
      else
        numlen = exp.length() - i;

      postfix += exp.substr(i, numlen) + ' ';
      i += numlen - 1;
    } else if (exp[i] == '(') {
      ops.push(exp[i]);
    } else if (exp[i] == ')') {
      while (ops.get() != '(') {
        (postfix += ops.pop()) += ' ';
      }
      ops.pop();
    } else if (exp[i] == '*' || exp[i] == '+' ||
        exp[i] == '-' || exp[i] == '/') {
      while (!ops.isEmpty() && priority(ops.get(), exp[i])) {
        (postfix += ops.pop()) += ' ';
      }
      ops.push(exp[i]);
    }
  }
  while (!ops.isEmpty()) {
    (postfix += ops.pop()) += ' ';
  }

  postfix.pop_back();  // delete space in the end
  return postfix;
}

// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 11.01.2021.
//
#ifndef INCLUDE_MYSTACK_H_
#define INCLUDE_MYSTACK_H_

#include <stdexcept>

template<typename T>
class MyStack {
 public:
  explicit MyStack(size_t cap = 10) :
      vals(new T[cap]),
      size(0),
      capacity(cap) {}
  MyStack(const MyStack<T>& other) : size(other.size),
                                     capacity(other.capacity),
                                     vals(new T[other.capacity]) {
    for (size_t i = 0; i < size; i++)
      vals[i] = other.vals[i];
  }
  ~MyStack() {
    delete[] vals;
  }
  T get() const {
    if (size > 0)
      return vals[size - 1];
    else
      throw std::out_of_range("Get call on empty stack!");
  }
  T pop() {
    if (size > 0)
      return vals[--size];
    else
      throw std::out_of_range("Pop call on empty stack!");
  }
  void push(T val) {
    if (size + 1 > capacity) {
      capacity *= 2;
      auto temp = new T[capacity];
      for (size_t i = 0; i < size; i++)
        temp[i] = vals[i];

      delete[] vals;
      vals = temp;
    }
    vals[size++] = val;
  }
  [[nodiscard]] bool isFull() const {
    return size == capacity;
  }
  [[nodiscard]] bool isEmpty() const {
    return size == 0;
  }

 private:
  T* vals;
  size_t size, capacity;
};

#endif  // INCLUDE_MYSTACK_H_

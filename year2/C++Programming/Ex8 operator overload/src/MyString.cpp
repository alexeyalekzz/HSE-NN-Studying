// Copyright (c) 2020 Alexey
//
// Created by Alexey on 07.12.2020.
//

#include <algorithm>
#include <cstring>
#include "MyString.h"

char* MyString::copyData(const char* str) {
  if (size == 0)
    return nullptr;

  data = new char[size + 1];
  for (size_t i{0}; i < size; i++)
    data[i] = str[i];
  data[size] = '\0';

  return data;
}

MyString::MyString(const char* str) : size(0), data(nullptr) {
  if (str == nullptr)
    return;

  for (size_t i{0}; str[i] != '\0'; i++)
    size++;

  data = copyData(str);
}
MyString::MyString(const std::string& str) : size(str.size()) {
  data = copyData(str.data());
}
MyString::MyString(const MyString& str) : size(str.size) {
  data = copyData(str.data);
}
MyString::MyString(MyString&& str) noexcept: data(str.data), size(str.size) {
  str.data = nullptr;
}

MyString::~MyString() {
  delete[] this->data;
}

int MyString::length() const {
  return this->size;
}

char* MyString::get() const {
  return this->data;
}

MyString MyString::operator+(const MyString& str) const {
  MyString temp;
  temp.size = this->size + str.size;
  temp.data = new char[temp.size + 1];

  int c{0};
  for (size_t i{0}; i < this->size; i++, c++)
    temp.data[c] = this->data[i];

  for (size_t i{0}; i < str.size; i++, c++)
    temp.data[c] = str[i];
  temp.data[temp.size] = '\0';

  return temp;
}
MyString MyString::operator+(const char c) const {
  MyString temp;
  temp.size = this->size + 1;
  temp.data = new char[temp.size + 1];

  for (size_t i{0}; i < this->size; i++)
    temp[i] = (*this)[i];
  temp[this->size] = c;
  temp[this->size + 1] = '\0';

  return temp;
}

MyString MyString::operator-(const MyString& str) {
  std::string temp;
  for (size_t i = 0; i < size; i++) {
    if (str((*this)[i]) == -1)
      temp += (*this)[i];
  }
  return MyString(temp);
}

char& MyString::operator[](size_t i) const {
  if (i >= 0 && i <= size)
    return data[i];
  else
    throw std::out_of_range("Index out of bounds");
}

MyString MyString::operator*(unsigned int i) const {
  MyString temp;
  temp.size = this->size * i;
  temp.data = new char[temp.size + 1];

  for (int j = 0; j < i; j++)
    std::copy(this->data, this->data + size, temp.data + j * size);
  temp[temp.size] = '\0';

  return temp;
}

MyString& MyString::operator=(const MyString& str) {
  if (&str == this)
    return *this;

  delete[] data;

  size = str.size;
  data = new char[size + 1];
  for (size_t i{0}; i < size; i++)
    data[i] = str[i];
  data[size] = '\0';

  return *this;
}
MyString& MyString::operator=(MyString&& str) noexcept {
  if (&str == this)
    return *this;

  delete[] data;

  size = str.size;
  data = str.data;
  str.data = nullptr;

  return *this;
}

bool MyString::operator==(const MyString& str) {
  if (!std::strcmp(this->data, str.data))
    return true;
  return false;
}

bool MyString::operator!=(const MyString& str) {
  if (std::strcmp(this->data, str.data))
    return true;
  return false;
}

bool MyString::operator>(const MyString& str) {
  if (std::strcmp(this->data, str.data) > 0)
    return true;
  return false;
}

bool MyString::operator<(const MyString& str) {
  if (std::strcmp(this->data, str.data) < 0)
    return true;
  return false;
}

bool MyString::operator>=(const MyString& str) {
  if (std::strcmp(this->data, str.data) >= 0)
    return true;
  return false;
}

bool MyString::operator<=(const MyString& str) {
  if (std::strcmp(this->data, str.data) <= 0)
    return true;
  return false;
}

MyString MyString::operator!() {
  MyString temp(*this);
  for (size_t i = 0; i < size; i++) {
    if (temp[i] >= 'a' && (*this)[i] <= 'z')
      temp[i] -= 32;
    else if (temp[i] >= 'A' && (*this)[i] <= 'Z')
      temp[i] += 32;
  }
  return temp;
}

int MyString::operator()(const char* sub) {
  std::string temp(this->data);
  size_t pos = temp.find(sub);
  if (pos != std::string::npos)
    return pos;
  else
    return -1;
}
int MyString::operator()(char sub) const {
  std::string temp(this->data);
  size_t pos = temp.find(sub);
  if (pos != std::string::npos)
    return pos;
  else
    return -1;
}

std::ostream& operator<<(std::ostream& os, const MyString& str) {
  return os << str.data;
}

std::istream& operator>>(std::istream& is, MyString& str) {
  std::string temp;
  is >> temp;
  str = MyString(temp);
  return is;
}

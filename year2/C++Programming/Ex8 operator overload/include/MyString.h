// Copyright (c) 2020 Alexey
//
// Created by Alexey on 07.12.2020.
//

#ifndef INCLUDE_MYSTRING_H_
#define INCLUDE_MYSTRING_H_

#include <string>
#include <iostream>

class MyString {
 private:
  char* data;
  size_t size;
  char* copyData(const char*);

 public:
  explicit MyString(const char* = nullptr);
  explicit MyString(const std::string&);
  MyString(const MyString&);
  MyString(MyString&&) noexcept;
  ~MyString();
  [[nodiscard]] int length() const;
  [[nodiscard]] char* get() const;

  MyString operator+(const MyString&) const;
  MyString operator+(char) const;
  MyString operator-(const MyString&);
  MyString operator*(unsigned int i) const;
  MyString& operator=(const MyString&);
  MyString& operator=(MyString&&) noexcept;
  bool operator==(const MyString&);
  bool operator!=(const MyString&);
  bool operator>(const MyString&);
  bool operator<(const MyString&);
  bool operator>=(const MyString&);
  bool operator<=(const MyString&);
  MyString operator!();
  char& operator[](size_t) const;
  int operator()(const char*);
  int operator()(char) const;
  friend std::istream& operator>>(std::istream&, MyString&);
  friend std::ostream& operator<<(std::ostream&, const MyString&);
};

#endif  // INCLUDE_MYSTRING_H_

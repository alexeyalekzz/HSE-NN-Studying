// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#ifndef INCLUDE_OBJECT_H_
#define INCLUDE_OBJECT_H_

#include <iostream>
#include "Common.h"

class Cell;


// Неживой обьект, знает только свой символ и тип
class Object {
 public:
  explicit Object(char sym, OBJECTS t) :
      symbol_(sym),
      type_(t),
      cell_(nullptr)
    {}
  virtual ~Object() = default;

  [[nodiscard]] OBJECTS type() const { return type_; }
  [[nodiscard]] inline char symbol() const { return symbol_; }

  friend Cell;

 protected:
  Cell* cell_;

 private:
  const OBJECTS type_;
  const char symbol_;
};

#endif  // INCLUDE_OBJECT_H_

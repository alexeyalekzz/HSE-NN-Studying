// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#include <vector>
#include <ctime>
#include <iostream>

#include "Cell.h"
#include "Ocean.h"


Cell::Cell() : obj_(nullptr) {}

Cell::~Cell() {
  delete obj_;
}

void Cell::init(size_t x, size_t y, Ocean* ocean) {
  x_ = x;
  y_ = y;
  ocean_ = ocean;
}

void Cell::move_to(Cell* to) {
  to->set_obj(this->obj_);
  this->free();
}

void Cell::set_obj(Object* o) {
  this->obj_ = o;
  o->cell_ = this;
}

void Cell::free() {
  this->obj_ = nullptr;
}

std::vector<Cell*> Cell::get_neighbourhood() const {
  std::vector<Cell*> result;

  for (int i = -1; i < 2; ++i)
    for (int j = -1; j < 2; ++j)
      if (y_ + j != -1 && y_ + j < ocean_->size_Y_ &&
          x_ + i != -1 && x_ + i < ocean_->size_X_ && (i || j))
        result.push_back(&((*ocean_)[x_ + i][y_ + j]));

  return result;
}

bool Cell::is_empty() const {
  return obj_ == nullptr;
}

OBJECTS Cell::creature_type() const {
  return obj_ == nullptr ? static_cast<OBJECTS>(-1) : obj_->type();
}

Object* Cell::get_object() const {
  return obj_;
}

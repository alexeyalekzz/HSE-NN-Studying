// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#ifndef INCLUDE_CELL_H_
#define INCLUDE_CELL_H_

#include <vector>

#include "Creature.h"


class Ocean;

class Cell {
 public:
  Cell();
  ~Cell();
  friend Ocean;

  void init(size_t, size_t, Ocean*);
  void set_obj(Object*);
  void move_to(Cell*);
  void free();

  [[nodiscard]] Object* get_object() const;
  [[nodiscard]] bool is_empty() const;
  [[nodiscard]] OBJECTS creature_type() const;
  [[nodiscard]] std::vector<Cell*> get_neighbourhood() const;

 private:
  Ocean* ocean_;
  Object* obj_;
  size_t x_, y_;
};

#endif  // INCLUDE_CELL_H_

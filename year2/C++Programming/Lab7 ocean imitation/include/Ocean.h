// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#ifndef INCLUDE_OCEAN_H_
#define INCLUDE_OCEAN_H_

#include <list>
#include "Cell.h"

class Ocean {
 public:
  const size_t size_X_, size_Y_;

  Ocean(unsigned, unsigned);
  ~Ocean();
  void run(int);
  inline void add_random_object(Cell* cell);
  void add_creature(Cell*, Creature*);
  Cell* operator[](size_t i);

 private:
  void print() const;

  Cell** field_;
  std::list<Creature*> all_creatures_;
  size_t iters_;
};

#endif  // INCLUDE_OCEAN_H_

// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 23.03.2021.
//

#ifndef INCLUDE_CREATURE_H_
#define INCLUDE_CREATURE_H_

#include "Object.h"

class Ocean;


// Живое создание, умеет ходить, размножаться, жить и умирать
class Creature : public Object {
 public:
  Creature(char sym, OBJECTS obj) :
      Object(sym, obj),
      lifetime_(0),
      is_dead_(false)
  {}

  virtual void live(Ocean*) = 0;
  virtual void move() = 0;
  virtual void reproduction(Ocean*) const = 0;
  virtual void die() = 0;

  [[nodiscard]] bool is_dead() const { return is_dead_; }

 protected:
  unsigned int lifetime_;
  bool is_dead_;
};

#endif  // INCLUDE_CREATURE_H_

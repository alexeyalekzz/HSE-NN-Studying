// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#ifndef INCLUDE_PREDATOR_H_
#define INCLUDE_PREDATOR_H_

#include "Prey.h"

class Predator : public Prey {
 public:
  explicit Predator(char = PREDATOR_SYM, OBJECTS = OBJECTS::PREDATOR);

  void live(Ocean*) override;

  //  Если рядом есть добыча - перемещается и съедает,
  //    иначе перемещается как Prey
  void move() override;

  //  Когда сыт, размножатеся как Prey
  void reproduction(Ocean*) const override;

 protected:
  [[nodiscard]] Creature * create_new() const override;
  bool fed_up_;
  unsigned int no_food_iters_;
};

#endif  // INCLUDE_PREDATOR_H_

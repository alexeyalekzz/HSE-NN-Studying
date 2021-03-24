// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#ifndef INCLUDE_PREY_H_
#define INCLUDE_PREY_H_

#include "Cell.h"

class Prey : public Creature {
 public:
  explicit Prey(char = PREY_SYM, OBJECTS = OBJECTS::PREY);

  void live(Ocean*) override;

  //  Перемещается в случайную свободную клетку поблизости
  void move() override;

  //  Создает в случайной свободной клекте поблизости объект
  void reproduction(Ocean*) const override;
  void die() override;

 protected:
  [[nodiscard]] virtual Creature* create_new() const;
};

#endif  // INCLUDE_PREY_H_

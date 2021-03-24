// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#include <vector>
#include <random>
#include <ctime>

#include "Predator.h"
#include "Common.h"
#include "Ocean.h"


Predator::Predator(char s, OBJECTS obj) :
    Prey(s, obj),
    fed_up_(false),
    no_food_iters_(0)
  {}

void Predator::live(Ocean* ocean) {
  if (lifetime_ <= PREDATOR_LIFETIME
      && no_food_iters_ <= PREDATOR_NO_FOOD_MAX
      && !is_dead_) {
    move();

    if (lifetime_ && lifetime_ % PREDATOR_REPR_TIME == 0)
      reproduction(ocean);

    lifetime_++;
  } else if (!is_dead_) {
    die();
  }
}

void Predator::move() {
  std::vector<Cell*> neighbours = cell_->get_neighbourhood();
  std::vector<Cell*> prey;

  for (auto cell : neighbours)
    if (!cell->is_empty() && cell->creature_type() == OBJECTS::PREY)
      prey.push_back(cell);

  if (!prey.empty()) {
    Cell* random_cell = prey[std::mt19937(time(nullptr))() % prey.size()];
    auto* random_prey = dynamic_cast<Creature*>(random_cell->get_object());
    random_prey->die();

    cell_->move_to(random_cell);
    fed_up_ = true;
    no_food_iters_ = 0;

  } else {
    fed_up_ = false;
    no_food_iters_++;
    Prey::move();
  }
}

void Predator::reproduction(Ocean* ocean) const {
  if (fed_up_)
    Prey::reproduction(ocean);
}

Creature* Predator::create_new() const {
  return new Predator;
}

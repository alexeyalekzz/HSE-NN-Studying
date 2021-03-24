// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#include <vector>
#include <random>
#include <ctime>

#include "Prey.h"
#include "Common.h"
#include "Ocean.h"


Prey::Prey(char s, OBJECTS obj) : Creature(s, obj) {}

void Prey::live(Ocean* ocean) {
  if (lifetime_ <= PREY_LIFETIME && !is_dead_) {
    move();

    if (lifetime_ && lifetime_ % PREY_REPR_TIME == 0)
      reproduction(ocean);

    lifetime_++;
  } else if (!is_dead_) {
    die();
  }
}

void Prey::move() {
  std::vector<Cell*> neighbours = cell_->get_neighbourhood();
  std::vector<Cell*> free_cells;

  for (auto& cell : neighbours)
    if (cell->is_empty())
      free_cells.push_back(cell);

  if (!free_cells.empty())
    cell_->move_to(free_cells[(std::mt19937(time(nullptr))()
                                                        % free_cells.size())]);
}

void Prey::reproduction(Ocean* ocean) const {
  std::vector<Cell*> neighbours = cell_->get_neighbourhood();
  std::vector<Cell*> free_cells;

  for (auto cell : neighbours)
    if (cell->is_empty())
      free_cells.push_back(cell);

  if (!free_cells.empty()) {
    Cell* random_cell = free_cells[(std::mt19937(time(nullptr))()
                                                          % free_cells.size())];
    ocean->add_creature(random_cell, this->create_new());
  }
}

void Prey::die() {
  cell_->free();
  is_dead_ = true;
}

Creature* Prey::create_new() const {
  return new Prey;
}

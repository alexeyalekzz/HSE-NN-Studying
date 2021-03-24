// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#include <random>
#include <ctime>
#include <iostream>

#include "Stone.h"
#include "Ocean.h"
#include "Common.h"
#include "Prey.h"
#include "Predator.h"


Ocean::Ocean(unsigned x, unsigned y) : size_X_(x), size_Y_(y), iters_(0) {
  field_ = new Cell*[size_X_];
  for (int i = 0; i < size_X_; ++i) {
    field_[i] = new Cell[size_Y_];
    for (int j = 0; j < size_Y_; ++j) {
      field_[i][j].init(i, j, this);
    }
  }

  for (int i = 0; i < size_X_; ++i) {
    for (int j = 0; j < size_Y_; ++j) {
      add_random_object(&field_[i][j]);
    }
  }
}

Ocean::~Ocean() {
  for (int i = 0; i < size_X_; ++i)
    delete[] field_[i];
  delete[] field_;
}


Cell* Ocean::operator[](size_t i) {
  return field_[i];
}

void Ocean::add_random_object(Cell* cell) {
  static std::mt19937 gen(std::time(nullptr));
  static std::uniform_int_distribution uid(0, OBJECTS_N + 1);

  switch (static_cast<OBJECTS>(uid(gen))) {
    case OBJECTS::STONE:
      field_[cell->x_][cell->y_].set_obj(new Stone);
      break;
    case OBJECTS::PREY:
      add_creature(cell, new Prey);
      break;
    case OBJECTS::PREDATOR:
      add_creature(cell, new Predator);
  }
}

void Ocean::add_creature(Cell* cell, Creature* creature) {
  cell->set_obj(creature);
  all_creatures_.push_back(creature);
}

void Ocean::print() const {
  std::string buf;
  for (int i = 0; i < size_X_; ++i) {
    for (int j = 0; j < size_Y_; ++j) {
      Object* obj = field_[i][j].obj_;
      if (obj != nullptr)
        buf += obj->symbol();
      else
        buf += '.';
    }
    buf += '\n';
  }
  std::cout << buf;
}

void Ocean::run(int max_iters) {
  while (max_iters-- > 0) {
    this->print();

    for (auto creature : all_creatures_)
      creature->live(this);

    // очистка памяти мертвых созданий
    // удаление указателей на них из списка
    auto it = all_creatures_.begin();
    while (it != all_creatures_.end()) {
      if ((*it)->is_dead()) {
        delete *it;
        it = all_creatures_.erase(it);
      } else {
        ++it;
      }
    }

    std::cout << iters_ + 1 << " iteration\n" << std::endl;
    iters_++;
  }
}

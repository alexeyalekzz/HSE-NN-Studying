// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#ifndef INCLUDE_STONE_H_
#define INCLUDE_STONE_H_

#include "Object.h"


class Stone : public Object {
 public:
  explicit Stone(char = STONE_SYM, OBJECTS = OBJECTS::STONE);
};

#endif  // INCLUDE_STONE_H_
